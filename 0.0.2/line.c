#include "line.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int line_get_char_count(Line* line) {
  return line->used;
}

void split_line(int position, Line* from, Line* to) {
  for (int from_index = position; from_index < from->used; from_index++) {
    add_char(to, from->value[from_index]);
    from->value[from_index] = '\0';
  }
  from->used -= from->used - position;
}

void insert_char(Line* line, char c, int position) {
  if (position == line->used) {
    add_char(line, c);
    return;
  }

  char old;
  if (position < line->used) {
    old = line->value[position];
    line->value[position] = c;
  }

  int current_size = line->used;

  for (int i = position + 1; i < current_size + 1; i++) {
    if (i == line->capacity) {
      line->capacity *= 2;
      line->value = realloc(line->value, line->capacity * sizeof(char));
    }
    char temp = line->value[i];
    line->value[i] = old;
    old = temp;
  }

  line->used++;
}

void delete_char(Line* line, char c, int position) {
  for (int i = position - 1; i < line->used; i++) {
    line->value[i] = line->value[i + 1];
  }

  line->value[line->used] = '\0';
  line->used -= 1;
}

void init_line(Line* line) {
  line->value = malloc(sizeof(char));
  line->capacity = 1;
  line->used = 0;
}

void add_char(Line* line, char c) {
  if (line->used == line->capacity) {
    line->capacity *= 2;
    line->value = realloc(line->value, line->capacity * sizeof(char));
  }

  line->value[line->used++] = c;
}

void free_line(Line* line) {
  free(line->value);
  line->value = NULL;
  line->capacity = line->used = 0;
}

void print_line(Line* line) {
  // fprintf(stdout, "[C]: %d, [U]: %d, [LINE]: %s\n", line->capacity, line->used, line->value);
}
