#include "line.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int line_get_char_count(Line* line) {
  return line->used;
}

/* TODO: 
 * - This should only take the line that should be split
 * - Create a new empty Line inside the function
 * - Append the characters to the empty line
 * - Return a pointer tp the new line
 * */
void line_split_line(int position, Line* from, Line* to) {
  for (int from_index = position; from_index < from->used; from_index++) {
    line_append_char(to, from->value[from_index]);
    from->value[from_index] = '\0';
  }
  from->used -= from->used - position;
}

void line_insert_char(Line* line, char c, int position) {
  if (position == line->used) {
    line_append_char(line, c);
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

void line_delete_char(Line* line, int position) {
  for (int i = position - 1; i < line->used; i++) {
    line->value[i] = line->value[i + 1];
  }

  line->value[line->used] = '\0';
  line->used -= 1;
}

Line* create_line() {
  Line* line = malloc(sizeof(Line));
  /* Memory allocation failed */
  if (NULL == line) return NULL;

  line->value = malloc(sizeof(char));
  if (NULL == line->value) {
    /* Memory allocation failed */
    free(line);
    return NULL;
  }

  line->capacity = 1;
  line->used = 0;

  return line;
}

void line_append_char(Line* line, char c) {
  if (line->used == line->capacity) {
    line->capacity *= 2;
    line->value = realloc(line->value, line->capacity * sizeof(char));
  }

  line->value[line->used++] = c;
}

void line_free(Line* line) {
  free(line->value);
  line->value = NULL;
  line->capacity = line->used = 0;
}

void line_print(Line* line) {
  // fprintf(stdout, "[C]: %d, [U]: %d, [LINE]: %s\n", line->capacity, line->used, line->value);
}
