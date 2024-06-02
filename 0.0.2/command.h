#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>

typedef struct {
  char* value;
  uint8_t used;
  uint8_t capacity;
} Command;

void init_command(Command* command);
void free_command(Command* command);

void append_char(Command* command, char c);
void insert_char(Command* command, char c, uint8_t position);
void remove_char(Command* command, uint8_t target);

#endif
