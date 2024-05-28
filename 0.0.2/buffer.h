#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>
#include <stdio.h>
#include "line.h"

typedef struct {
  Line* lines;
  uint8_t capacity;
  uint8_t used;
} Buffer;

void load_file(Buffer* buffer, FILE* file);
void buffer_add_ch(Buffer* buffer, char c, int line, int position);

void buffer_append_empty_line_to_end();
void buffer_insert_empty_line_after(int line);

void init_buffer(Buffer* buffer);
void add_line(Buffer* buffer, Line line);
void free_buffer(Buffer* buffer);
void print_buffer(Buffer* buffer);

#endif
