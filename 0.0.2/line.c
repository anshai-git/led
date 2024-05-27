#include "line.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void init_line(Line* line) {
  line->value = malloc(sizeof(char));
  line->capacity = 1;
  line->used = 0;
}

void add_char(Line* line, char c) {
  if (line->used == line->capacity) {
    line->capacity *= 2;
    line->value = realloc(line->value, line->capacity * sizeof(Line));
  }

  line->value[line->used++] = c;
}

void free_line(Line* line) {
  free(line->value);
  line->value = NULL;
  line->capacity = line->used = 0;
}

void print_line(Line* line) {
  fprintf(stdout, "[LINE]: %s", line->value);
}
