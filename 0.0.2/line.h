#ifndef _LINE_H_
#define _LINE_H_

#include <stdint.h>
#include <stdint.h>

typedef struct {
  char* value;
  uint8_t capacity;
  uint8_t used;
} Line;

void init_line(Line* line);
void add_char(Line* line, char c);
void free_line(Line* line);
void print_line(Line* line);

#endif
