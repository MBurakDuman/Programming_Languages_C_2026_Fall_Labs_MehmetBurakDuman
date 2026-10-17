/*
 * Name: Mehmet Burak Duman
 * Student ID: 241ADB160
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int n;

  printf("Enter number of elements: ");

  /* scanf sonucu 1 degilse sayi okunamadi.
     || = veya. Gecersiz giriste n kontrol edilmez. */
  if (scanf("%d", &n) != 1 || n <= 0) {
    printf("Invalid size.\n");
    return 1;
  }

  /* Eleman sayisi * eleman boyutu tasmasini onle. */
  if ((size_t)n > SIZE_MAX / sizeof(int)) {
    printf("Memory allocation failed.\n");
    return 1;
  }

  /* malloc byte cinsinden alan ayirir.
     arr, ayrilan alanin adresini tutar. */
  int* arr = malloc((size_t)n * sizeof(int));

  if (arr == NULL) {
    printf("Memory allocation failed.\n");
    return 1;
  }

  /* Toplami donguden once sifirla.
     long long, int'e gore daha buyuk toplam saklar. */
  long long sum = 0;

  printf("Enter %d integers: ", n);

  /* Dizilerde ilk indeks 0, son indeks n - 1.
     Bu nedenle i < n kullanilir. */
  for (int i = 0; i < n; i++) {
    if (scanf("%d", &arr[i]) != 1) {
      free(arr);
      printf("Invalid input.\n");
      return 1;
    }

    sum = sum + arr[i];
  }

  /* (double), bolmenin ondalikli yapilmasini saglar.
     Aksi halde tam sayi bolmesi kesir kismini atar. */
  double average = (double)sum / n;

  printf("Sum = %lld\n", sum);
  printf("Average = %.2f\n", average);

  /* Basarili malloc ile alinan bellegi geri ver. */
  free(arr);

  return 0;
}