/*
 * Name: Mehmet Burak
 * Surname: Duman
 * Student ID: 241ADB160
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

int save_student_binary(const Student* s, const char* filename);
int load_student_binary(const char* filename, Student* out);
long file_size(const char* filename);

int main(void) {
  /* Kullanilmayan isim karakterlerini de sifirla. */
  Student s = {0};
  strcpy(s.name, "Alice");
  s.age = 21;
  s.gpa = 3.75f;

  Student loaded = {0};

  printf("Saving student in binary format...\n");

  if (save_student_binary(&s, "student.bin") != 0) {
    fprintf(stderr, "Could not save binary student.\n");
    return EXIT_FAILURE;
  }

  printf("Loading student from binary file...\n");

  if (load_student_binary("student.bin", &loaded) != 0) {
    fprintf(stderr, "Could not load binary student.\n");
    return EXIT_FAILURE;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);

  /* Karsilastirma icin ayni kaydi metin olarak da yaz. */
  FILE* fp = fopen("student.txt", "w");

  if (fp == NULL) {
    perror("Cannot open text file");
    return EXIT_FAILURE;
  }

  int written = fprintf(fp, "%s %d %.2f\n", s.name, s.age, s.gpa);

  int closed = fclose(fp);

  if (written < 0 || closed != 0) {
    fprintf(stderr, "Could not write text student.\n");
    return EXIT_FAILURE;
  }

  long text_bytes = file_size("student.txt");
  long binary_bytes = file_size("student.bin");

  if (text_bytes < 0 || binary_bytes < 0) {
    fprintf(stderr, "Could not determine file sizes.\n");
    return EXIT_FAILURE;
  }

  printf("Text file size: %ld bytes\n", text_bytes);
  printf("Binary file size: %ld bytes\n", binary_bytes);

  return EXIT_SUCCESS;
}

int save_student_binary(const Student* s, const char* filename) {
  /* wb = write binary */
  FILE* fp = fopen(filename, "wb");

  if (fp == NULL) {
    return -1;
  }

  /* sizeof(Student) byte buyuklugunde 1 kayit yazar.
     Sonuc, yazilan kayit sayisidir; byte sayisi degildir. */
  size_t records = fwrite(s, sizeof(Student), 1, fp);
  int closed = fclose(fp);

  if (records != 1 || closed != 0) {
    return -1;
  }

  return 0;
}

int load_student_binary(const char* filename, Student* out) {
  /* rb = read binary */
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL) {
    return -1;
  }

  Student temp = {0};

  size_t records = fread(&temp, sizeof(Student), 1, fp);
  int closed = fclose(fp);

  if (records != 1 || closed != 0) {
    return -1;
  }

  /* Bozuk dosyada da isim dizisinin sonu garanti olsun. */
  temp.name[MAX_NAME_LEN - 1] = '\0';

  *out = temp;
  return 0;
}

long file_size(const char* filename) {
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL) {
    return -1;
  }

  /* Konumu dosyanin sonuna tasi. */
  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return -1;
  }

  /* Bastan itibaren byte konumu = dosya boyutu. */
  long bytes = ftell(fp);
  int closed = fclose(fp);

  if (bytes < 0 || closed != 0) {
    return -1;
  }

  return bytes;
}