#include "util.h"

#include <stdio.h>
#include <stdlib.h>

char *readFile(char *filename) {
  FILE *f;
  fopen_s(&f, filename, "rb");

  if (f == NULL) {
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buf = malloc(fsize + 1);
  fread(buf, fsize, 1, f);
  fclose(f);

  buf[fsize] = 0;

  return buf;
}

void freeFile(char *content) { free(content); }
