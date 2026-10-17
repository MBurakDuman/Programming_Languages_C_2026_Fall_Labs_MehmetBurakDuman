/*
 * Name: Mehmet Burak
 * Surname: Duman
 * Student ID: 241ADB160
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

int load_students(Student arr[], int max);
int save_students(const Student arr[], int count);
void add_student(Student arr[], int* count);
void list_students(const Student arr[], int count);
int read_line(const char* prompt, char* buf, size_t size);

static int valid_name(const char* name);
static int parse_id(const char* text, int* out);
static int parse_gpa(const char* text, float* out);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  char input[LINE_LEN];
  int running = 1;

  count = load_students(students, MAX_STUDENTS);
  printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

  while (running) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");

    int choice;

    if (read_line("Select an option: ", input, sizeof input) != 0) {
      choice = 3;
    } else {
      char* end;
      long value = strtol(input, &end, 10);
      choice = (end != input && *end == '\0') ? (int)value : 0;
    }

    switch (choice) {
      case 1:
        list_students(students, count);
        break;

      case 2:
        add_student(students, &count);
        break;

      case 3:
        if (save_students(students, count) != 0) {
          fprintf(stderr, "Could not save students.\n");
          return EXIT_FAILURE;
        }

        printf("Data saved. Goodbye!\n");
        running = 0;
        break;

      default:
        printf("Invalid option. Try again.\n");
    }
  }

  return EXIT_SUCCESS;
}

/* Verilen yardimci: satir okur ve sondaki Enter'i kaldirir. */
int read_line(const char* prompt, char* buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);

  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }

  size_t len = strcspn(buf, "\r\n");

  if (buf[len] == '\0' && len == size - 1) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }

  buf[len] = '\0';
  return 0;
}

/* Isim bos olamaz, sadece bosluklardan olusamaz veya virgul iceremez. */
static int valid_name(const char* name) {
  if (name[0] == '\0' || strchr(name, ',') != NULL) {
    return 0;
  }

  for (int i = 0; name[i] != '\0'; i++) {
    if (!isspace((unsigned char)name[i])) {
      return 1;
    }
  }

  return 0;
}

/* Metnin tamamini kontrol ederek int degerine cevirir.
   out bir adres: sonucu *out ile o adrese kaydederiz. */
static int parse_id(const char* text, int* out) {
  char* end;
  errno = 0;

  long value = strtol(text, &end, 10);

  if (end == text || errno != 0) {
    return 0;
  }

  while (isspace((unsigned char)*end)) {
    end++;
  }

  if (*end != '\0' || value < INT_MIN || value > INT_MAX) {
    return 0;
  }

  *out = (int)value;
  return 1;
}

/* GPA sayi olmali ve 0-4 araliginda bulunmali. */
static int parse_gpa(const char* text, float* out) {
  char* end;
  errno = 0;

  float value = strtof(text, &end);

  if (end == text || errno != 0) {
    return 0;
  }

  while (isspace((unsigned char)*end)) {
    end++;
  }

  if (*end != '\0' || !isfinite(value) || value < 0.0f || value > 4.0f) {
    return 0;
  }

  *out = value;
  return 1;
}

int load_students(Student arr[], int max) {
  FILE* fp = fopen(DATA_FILE, "r");

  /* Dosya henuz yoksa ilk calistirma: sifir kayit. */
  if (fp == NULL) {
    return 0;
  }

  char line[LINE_LEN];
  int count = 0;

  while (count < max && fgets(line, sizeof(line), fp) != NULL) {
    /* Tampona sigmayan satirin tamamini atla. */
    if (strchr(line, '\n') == NULL && !feof(fp)) {
      int ch;

      while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
      }

      continue;
    }

    line[strcspn(line, "\r\n")] = '\0';

    /* Beklenen bicim: name,id,gpa */
    char* first = strchr(line, ',');

    if (first == NULL) {
      continue;
    }

    char* second = strchr(first + 1, ',');

    if (second == NULL || strchr(second + 1, ',') != NULL) {
      continue;
    }

    /* Virgulleri metin bitis isaretine cevir:
       boylece uc ayri metin parcasi elde edilir. */
    *first = '\0';
    *second = '\0';

    Student temp = {0};

    if (strlen(line) >= NAME_LEN || !valid_name(line) ||
        !parse_id(first + 1, &temp.id) || !parse_gpa(second + 1, &temp.gpa)) {
      continue;
    }

    strcpy(temp.name, line);
    arr[count] = temp;
    count++;
  }

  fclose(fp);
  return count;
}

int save_students(const Student arr[], int count) {
  /* "w": dosyayi mevcut kayitlarla yeniden yazar. */
  FILE* fp = fopen(DATA_FILE, "w");

  if (fp == NULL) {
    return -1;
  }

  int failed = 0;

  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      failed = 1;
      break;
    }
  }

  if (fclose(fp) != 0) {
    failed = 1;
  }

  return failed ? -1 : 0;
}

void add_student(Student arr[], int* count) {
  /* Pointer ile gelen sayaci *count kullanarak okuruz. */
  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  Student temp = {0};
  char input[LINE_LEN];

  if (read_line("Enter name: ", input, sizeof(input)) != 0) {
    return;
  }

  if (strlen(input) >= NAME_LEN || !valid_name(input)) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  strcpy(temp.name, input);

  if (read_line("Enter ID: ", input, sizeof(input)) != 0) {
    return;
  }

  if (!parse_id(input, &temp.id)) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  if (read_line("Enter GPA: ", input, sizeof(input)) != 0) {
    return;
  }

  if (!parse_gpa(input, &temp.gpa)) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  /* Once dogrula, sonra kaydet ve sayaci artir. */
  arr[*count] = temp;
  (*count)++;

  printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  if (count == 0) {
    printf("No students found.\n");
    return;
  }

  printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}