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

void line_insert_char(Line* line, char c, int position);

void line_delete_char(Line* line, int position);

/* Append all the character from index @position in line A to line B */
void line_split_line(int position, Line* A, Line* B);

/* Allocate memory for a Line and return a pointer to it */
Line* create_line();

/* Append the character under @c to the end of the line */
void line_append_char(Line* line, char c);

/* Free the allocated memory in the line */
void line_free(Line* line);

/* Print a string representation of the line to stdout */
void line_print(Line* line);

#endif
