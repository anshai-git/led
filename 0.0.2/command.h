#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>

typedef struct {
  char* value;
  uint8_t used;
  uint8_t capacity;
} Command;

/* Allocate memory for a Command and return a pointer to it */
Command* create_command();
void command_free(Command* command);

void command_append_char(Command* command, char c);
void command_insert_char(Command* command, char c, uint8_t position);
void command_remove_char(Command* command, uint8_t target);

#endif
