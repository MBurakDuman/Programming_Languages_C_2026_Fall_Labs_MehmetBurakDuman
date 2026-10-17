/*
 * Name: Mehmet Burak Duman
 * Student ID: 241ADB160
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    int n;

    printf("Enter number of students: ");

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number.\n");
        return 1;
    }

    if ((size_t)n > SIZE_MAX / sizeof(struct Student)) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* n adet tam ogrenci kaydi icin alan ayir.
       sizeof(struct Student), tum alanlarin kapladigi boyuttur. */
    struct Student *students =
        malloc((size_t)n * sizeof(struct Student));

    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter data for student %d: ", i + 1);

        /* name: karakter dizisi, onune & gelmez.
           id: int, scanf icinde %d ve & gerekir.
           grade: float, scanf icinde %f ve & gerekir.
           %49s: name[50] icindeki bitis isaretine yer birakir.
           Uc alan beklendiginden scanf sonucu 3 olmali. */
        if (scanf("%49s %d %f",
                  students[i].name,
                  &students[i].id,
                  &students[i].grade) != 3) {
            free(students);
            printf("Invalid input.\n");
            return 1;
        }
    }

    printf("\n");

    /* Eksi isareti sola hizalar.
       6 ve 11, alanlarin minimum sutun genisligidir. */
    printf("%-6s %-11s %s\n", "ID", "Name", "Grade");

    for (int i = 0; i < n; i++) {
        printf("%-6d %-11s %.1f\n",
               students[i].id,
               students[i].name,
               students[i].grade);
    }

    free(students);

    return 0;
}