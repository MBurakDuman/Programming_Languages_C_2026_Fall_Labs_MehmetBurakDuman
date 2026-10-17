/*
 * Name: Mehmet Burak
 * Surname: Duman
 * Student ID: 241ADB160
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

/* typedef sayesinde "struct Student" yerine Student yazabiliriz. */
typedef struct Student {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

int save_student(const Student* s, const char* filename);
int load_student(const char* filename, Student* out);

int main(void) {
  Student s1 = {0};
  strcpy(s1.name, "Alice");
  s1.age = 21;
  s1.gpa = 3.75f;

  Student loaded = {0};
  const char* filename = "student.txt";

  printf("Saving student to file...\n");

  if (save_student(&s1, filename) != 0) {
    fprintf(stderr, "Could not save student.\n");
    return EXIT_FAILURE;
  }

  printf("Loading student from file...\n");

  if (load_student(filename, &loaded) != 0) {
    fprintf(stderr, "Could not load student.\n");
    return EXIT_FAILURE;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);

  return EXIT_SUCCESS;
}

int save_student(const Student* s, const char* filename) {
  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    return -1;
  }

  /* s bir pointer oldugu icin alanlara -> ile erisilir.
     const, s uzerinden ogrenci bilgisini degistirmemizi engeller. */
  int written = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);

  int closed = fclose(fp);

  if (written < 0 || closed != 0) {
    return -1;
  }

  return 0;
}

int load_student(const char* filename, Student* out) {
  FILE* fp = fopen(filename, "r");

  if (fp == NULL) {
    return -1;
  }

  /* Gecici kayit: okuma basarisizsa out'u degistirme. */
  Student temp = {0};

  /* float okurken %f, double okurken %lf.
     name bir dizi: & yok; sayisal alanlarda & var. */
  int fields = fscanf(fp, "%49s %d %f", temp.name, &temp.age, &temp.gpa);

  int closed = fclose(fp);

  if (fields != 3 || closed != 0) {
    return -1;
  }

  *out = temp;
  return 0;
}