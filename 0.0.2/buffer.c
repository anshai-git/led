#include "buffer.h"
#include "line.h"

#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void buffer_add_ch(Buffer *buffer, char c, int line, int position) {
  insert_char(&buffer->lines[line], c, position);
}

void buffer_del_ch(Buffer *buffer, char c, int line, int position) {
  delete_char(&buffer->lines[line], c, position);
}

void join_lines(Buffer *buffer, int source_index, int target_index) {
  for (int i = 0; i < buffer->lines[source_index].used; i++) {
    add_char(&buffer->lines[target_index],
             buffer->lines[source_index].value[i]);
  }
}

void remove_line(Buffer *buffer, int target_index) {
  for (int i = target_index; i < buffer->used; i++) {
    buffer->lines[i] = buffer->lines[i + 1];
  }

  free_line(&buffer->lines[buffer->used]);
  buffer->used -= 1;
}

void insert_line(Buffer *buffer, Line line, int position) {
  position += 1; // FIXME

  if (position == buffer->used) {
    add_line(buffer, line);
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

/*
 * Read the file and load it into the buffer
 * */
void load_file(Buffer *buffer, FILE *file) {
  Line line;
  init_line(&line);

  char c;
  while ((c = fgetc(file)) != EOF) {
    add_char(&line, c);
    if (c == '\n') {
      add_line(buffer, line);
      init_line(&line);
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

  if (NULL == buffer) {
    /* Failed to allocate memory for buffer */
    return NULL;
  }

  buffer->lines = malloc(sizeof(Line));
  if (NULL == buffer->lines) {
    /* Failed to allocate memory for lines */
    free(buffer);
    return NULL;
  }

  buffer->capacity = 1;
  buffer->used = 0;

  return buffer;
}

void add_line(Buffer *buffer, Line line) {
  if (buffer->used == buffer->capacity) {
    buffer->capacity *= 2;
    buffer->lines = realloc(buffer->lines, buffer->capacity * sizeof(Line));
  }
  buffer->lines[buffer->used++] = line;
}

void free_buffer(Buffer *buffer) {
  uint8_t index = 0;
  while (index < buffer->used) {
    free_line(&buffer->lines[index++]);
  }
  free(buffer->lines);
  buffer->lines = NULL;
  buffer->capacity = 0;
  buffer->used = 0;
}

void print_buffer(Buffer *buffer) {
  if (buffer->used == 0) {
    fprintf(stdout, "This buffer doesn't containt any data yet.\n");
    return;
  }

  uint8_t index = 0;
  while (index < buffer->used) {
    print_line(&buffer->lines[index++]);
  }
}
