/*
 * Name: Mehmet Burak Duman
 * Student ID: 241ADB160
 */

#include <stdio.h>

/* struct, farkli turde bilgileri tek kayitta toplar.
   Tanim sonundaki noktali virgul zorunludur. */
struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    /* Alanlar sirayla: name, id, grade.
       Baslangicta metin bu sekilde atanabilir. */
    struct Student student1 = {"Alice Johnson", 1001, 9.1f};
    struct Student student2 = {"Bob Smith", 1002, 8.7f};

    /* Nokta operatoru kaydin alanina erisir.
       %.1f, bir ondalik basamak yazdirir. */
    printf("Student 1: %s, ID: %d, Grade: %.1f\n",
           student1.name, student1.id, student1.grade);

    printf("Student 2: %s, ID: %d, Grade: %.1f\n",
           student2.name, student2.id, student2.grade);

    return 0;
}