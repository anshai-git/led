#ifndef _LINE_H_
#define _LINE_H_

#include <stdint.h>
#include <stdint.h>

typedef struct {
  char* value;
  uint8_t capacity;
  uint8_t used;
} Line;

int line_get_char_count(Line* line);

void insert_char(Line* line, char c, int position);

void delete_char(Line* line, char c, int position);

void split_line(int position, Line* from, Line* to);

void init_line(Line* line);

void add_char(Line* line, char c);

void free_line(Line* line);

void print_line(Line* line);

#endif
