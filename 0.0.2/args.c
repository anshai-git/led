#include <stdio.h>

/*
 * TODO: write a reasonable argument parser
 * */
char* args_parse_filename(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "File path missing\nUsage: %s <file path>\nExample: %s input.txt", argv[0], argv[0]);
    return NULL;
  }

  if (argc == 2) {
    return argv[1];
  }

  if (argc > 2) {
    fprintf(stderr, "Invalid number of arguments.");
    return NULL;
  }

  return NULL;
}
