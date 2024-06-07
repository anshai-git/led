#include "command.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>

Command* create_command() {
  Command* command = malloc(sizeof(Command));

  /* Memory allocation failed */
  if (NULL == command) return NULL;

  command->value = malloc(sizeof(char));
  /* Memory allocation failed */
  if (NULL == command->value) {
    free(command);
    return NULL;
  }

  command->capacity = 1;
  command->used = 0;

  return command;
}

void free_command(Command* command) {
  command->used = 0;
  command->capacity = 0;

  free(command->value);
  command->value = NULL;
}

void command_append_char(Command* command, char c) {
  if (command->capacity == command->used) {
    command->capacity *= 2;
    command->value = realloc(command->value, command->capacity * sizeof(char));
  }
  command->value[command->used] = c;
  command->used += 1;
}

void command_insert_char(Command* command, char c, uint8_t position) {
  /* @Continue */
}

void command_remove_char(Command* command, uint8_t target) {
  /* @Continue */
}
