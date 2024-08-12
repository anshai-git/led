#include <stdio.h>

FILE* open_or_create_new_file(const char* path) {
  FILE* file = fopen(path, "rw");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return NULL;
  }
  return file;
}
