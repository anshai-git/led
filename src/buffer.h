#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>
#include <stdio.h>
#include "line.h"

typedef struct {
  Line* lines;
  uint16_t capacity;
  uint16_t used;
  uint16_t cursor_x;
  uint16_t cursor_y;
} Buffer;

/* Read the file and load it into the buffer */
void buffer_load_file(Buffer* buffer, FILE* file);

/* Insert one character in the line undex @line_index in the position undex @char_index */
void buffer_insert_ch(Buffer* buffer, char c, int line_index, int char_index);

/* Delete the character in the line under @line_index positioned under @char_index */
void buffer_delete_ch(Buffer* buffer, int line_index, int char_index);

/* Insert the line under @line under the index @position */
void buffer_insert_line(Buffer* buffer, Line line, int position);

/*
 * Append the line under 'source_index' to the line under 'target_index'
 * Shift all lines after target_index backwards accordingly
 * */
void buffer_join_lines(Buffer* buffer, int source_index, int target_index);

/*
 * Remove the line under 'target_index'
 * Shift all lines after target_index backwards accordingly
 * */
void buffer_remove_line(Buffer* buffer, int target_index);

/* Insert an empty line to the end of the buffer */
void buffer_append_empty_line_to_end(Buffer* buffer);

/* Insert an empty line after the line under @line_index */
void buffer_insert_empty_line_after(Buffer* buffer, int line_index);

/* Returns the number of lines in the buffer */
int buffer_get_line_count(Buffer* buffer);

/* Returns the number of characters in the line under @line_index */
int buffer_get_line_length(Buffer* buffer, int line_index);

/* Allocate memory for a Buffer */
Buffer* create_buffer();

/* Append one line to the end of the buffer */
void buffer_append_line(Buffer* buffer, Line line);

/* Free the allocated memory in the buffer */
void buffer_free(Buffer* buffer);

/* Print a string representation of the buffer to stdout */
void buffer_print(Buffer* buffer);

#endif
