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

void buffer_del_ch(Buffer* buffer, char c, int line, int position);

void insert_line(Buffer* buffer, Line line, int position);

/*
 * Append the line under 'source_index' to the line
 * under 'target_index'
 * */
void join_lines(Buffer* buffer, int source_index, int target_index);

/*
 * Remove the line under 'target_index'
 * Shitf all lines back accordingly
 * */
void remove_line(Buffer* buffer, int target_index);

void buffer_append_empty_line_to_end(Buffer* buffer);

void buffer_insert_empty_line_after(Buffer* buffer, int line);

int buffer_get_line_count(Buffer* buffer);

int buffer_get_line_length(Buffer* buffer, int line_index);

void init_buffer(Buffer* buffer);

void add_line(Buffer* buffer, Line line);

void free_buffer(Buffer* buffer);

void print_buffer(Buffer* buffer);

#endif
