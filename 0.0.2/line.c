#include "line.h"
#include <stdlib.h>

void init_line(Line *line) {
  line->value = malloc(256 * sizeof(char));
  line->size = 0;
}

void add_char(Line *line, char c) {
  line->value[line->size++] = c;
}

void free_line(Line *line) {
  free(line->value);
  line->value = NULL;
  line->capacity = 0;
  line->used = 0;
}
