#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>
#include "line.h"

typedef struct {
  Line* lines;
  uint8_t capacity;
  uint8_t used;
} Buffer;

void init_buffer(Buffer* buffer);
void add_line(Buffer* buffer, Line line);
void free_buffer(Buffer* buffer);
void print_buffer(Buffer* buffer);

#endif
