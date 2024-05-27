#include "buffer.h"
#include "line.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void init_buffer(Buffer* buffer) {
  buffer->lines = malloc(sizeof(Line));
  buffer->capacity = 1;
  buffer->used = 0;
}

void add_line(Buffer* buffer, Line line) {
  if (buffer->used == buffer->capacity) {
    buffer->capacity *= 2;
    buffer->lines = realloc(buffer->lines, buffer->capacity * sizeof(Line));
  }
  buffer->lines[buffer->used++] = line;
}

void free_buffer(Buffer* buffer) {
  uint8_t index = 0;
  while (index < buffer->used) {
    free_line(&buffer->lines[index++]);
  }
  free(buffer->lines);
  buffer->lines = NULL;
  buffer->capacity = 0;
  buffer->used = 0;
}

void print_buffer(Buffer* buffer) {
  if (buffer->used == 0) {
    fprintf(stdout, "This buffer doesn't containt any data yet.\n");
    return;
  }

  uint8_t index = 0;
  while (index < buffer->used) {
    print_line(&buffer->lines[index++]);
  }
}
