/*
 * Name: Mehmet Burak
 * Surname: Duman
 * Student ID: 241ADB160
 */

#include <stdio.h>

int main(int argc, char* argv[]) {
  /* argv[1], varsa kullanicinin verdigi dosya adidir.
     Yoksa varsayilan data.txt kullanilir. */
  const char* filename = "data.txt";

  if (argc > 1) {
    filename = argv[1];
  }

  printf("Writing lines to %s...\n", filename);

  /* "w": dosyayi olusturur, varsa eski icerigini siler. */
  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    perror("Cannot open file for writing");
    return 1;
  }

  /* fprintf: printf gibi, fakat ekrana degil dosyaya yazar. */
  int failed = 0;

  if (fprintf(fp, "Hello, file I/O in C!\n") < 0) {
    failed = 1;
  }

  if (fprintf(fp, "This is another line.\n") < 0) {
    failed = 1;
  }

  /* fclose, bekleyen verilerin yazilmasinda da hata verebilir. */
  if (fclose(fp) != 0) {
    failed = 1;
  }

  if (failed) {
    fprintf(stderr, "File writing failed.\n");
    return 1;
  }

  /* "r": mevcut dosyayi okumak icin acar. */
  fp = fopen(filename, "r");

  if (fp == NULL) {
    perror("Cannot open file for reading");
    return 1;
  }

  printf("Reading contents:\n");

  char line[256];
  int count = 0;

  /* fgets satir okur; EOF veya hatada NULL dondurur.
     Satirdaki newline korundugu icin ekstra newline eklemiyoruz. */
  while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
    count++;
  }

  failed = ferror(fp);

  if (fclose(fp) != 0) {
    failed = 1;
  }

  if (failed) {
    fprintf(stderr, "File reading failed.\n");
    return 1;
  }

  printf("Lines read: %d\n", count);

  return 0;
}