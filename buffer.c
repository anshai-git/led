#include "buffer.h"
#include "line.h"

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void buffer_insert_ch(Buffer *buffer, char c, int line, int position) {
  line_insert_char(&buffer->lines[line], c, position);
}

void buffer_delete_ch(Buffer *buffer, int line_index, int char_index) {
  line_delete_char(&buffer->lines[line_index], char_index);
}

void buffer_join_lines(Buffer *buffer, int source_index, int target_index) {
  for (int i = 0; i < buffer->lines[source_index].used; i++) {
    line_append_char(&buffer->lines[target_index],
             buffer->lines[source_index].value[i]);
  }
}

void buffer_remove_line(Buffer *buffer, int target_index) {
  for (int i = target_index; i < buffer->used; i++) {
    buffer->lines[i] = buffer->lines[i + 1];
  }

  line_free(&buffer->lines[buffer->used]);
  buffer->used -= 1;
}

void buffer_insert_line(Buffer *buffer, Line line, int position) {
  position += 1; // FIXME

  if (position == buffer->used) {
    buffer_append_line(buffer, line);
    return;
  }

  Line old;
  if (position < buffer->used) {
    old = buffer->lines[position];
    buffer->lines[position] = line;
  }

  int current_size = buffer->used;
  for (int i = position + 1; i < current_size + 1; i++) {
    if (i == buffer->capacity) {
      buffer->capacity *= 2;
      buffer->lines = realloc(buffer->lines, buffer->capacity * sizeof(Line));
    }

    Line temp = buffer->lines[i];
    buffer->lines[i] = old;
    old = temp;
  }

  buffer->used += 1;
}

void buffer_load_file(Buffer *buffer, FILE *file) {
  Line* line = create_line();
  char c;
  while ((c = fgetc(file)) != EOF) {
    line_append_char(line, c);
    if (c == '\n') {
      buffer_append_line(buffer, *line);
      line = create_line();
    }
  }
}

void buffer_append_empty_line_to_end(Buffer* buffer) {
  /* TODO:
   * Not implemented
   * Why would I even need it?
   * */
}

void buffer_insert_empty_line_after(Buffer* buffer, int line) {
  /* TODO:
   * Not implemented
   * Why would I even need it?
   * */
}

int buffer_get_line_count(Buffer* buffer) {
  return buffer->used;
}

int buffer_get_line_length(Buffer* buffer, int line_index) {
  return line_get_char_count(&buffer->lines[line_index]);
}

Buffer* create_buffer() {
  Buffer* buffer = malloc(sizeof(Buffer));
  /* Failed to allocate memory for buffer */
  if (NULL == buffer) return NULL;

  buffer->lines = malloc(sizeof(Line));
  /* Failed to allocate memory for lines */
  if (NULL == buffer->lines) {
    free(buffer);
    return NULL;
  }

  buffer->capacity = 1;
  buffer->used = 0;

  return buffer;
}

/* TODO:
 * Should this use a pointer to a Line instead?!
 * */
void buffer_append_line(Buffer *buffer, Line line) {
  if (buffer->used == buffer->capacity) {
    buffer->capacity *= 2;
    buffer->lines = realloc(buffer->lines, buffer->capacity * sizeof(Line));
  }
  buffer->lines[buffer->used++] = line;
}

void buffer_free(Buffer *buffer) {
  uint8_t index = 0;
  while (index < buffer->used) {
    line_free(&buffer->lines[index++]);
  }
  free(buffer->lines);
  buffer->lines = NULL;
  buffer->capacity = 0;
  buffer->used = 0;
}

void buffer_print(Buffer *buffer) {
  if (buffer->used == 0) {
    fprintf(stdout, "This buffer doesn't containt any data yet.\n");
    return;
  }

  uint8_t index = 0;
  while (index < buffer->used) {
    line_print(&buffer->lines[index++]);
  }
}
