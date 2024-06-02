#include "command.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>

void init_command(Command* command) {
  command->capacity = 1;
  command->used = 0;
  command->value = malloc(sizeof(char));
}

void free_command(Command* command) {
  command->used = 0;
  command->capacity = 0;

  free(command->value);
  command->value = NULL;
}

void append_char(Command* command, char c) {
  if (command->capacity == command->used) {
    command->capacity *= 2;
    command->value = realloc(command->value, command->capacity * sizeof(char));
  }
  command->value[command->used] = c;
  command->used += 1;
}

void insert_char(Command* command, char c, uint8_t position) {
  /* @Continue */
}

void remove_char(Command* command, uint8_t target) {
  /* @Continue */
}
