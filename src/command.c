#include "command.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Command *create_command() {
  Command *command = malloc(sizeof(Command));

  /* Memory allocation failed */
  if (NULL == command)
    return NULL;

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

void free_command(Command *command) {
  command->used = 0;
  command->capacity = 1;

  free(command->value);
  command->value = NULL;
}

void command_append_char(Command *command, char c) {
  if (command->capacity == command->used) {
    command->capacity *= 2;
    command->value = realloc(command->value, command->capacity * sizeof(char));
  }
  command->value[command->used] = c;
  command->used += 1;
}

void command_insert_char(Command *command, char c, uint8_t position) {
  if (position == command->used) {
    command_append_char(command, c);
    return;
  } 

  if (command->capacity == command->used) {
    command->capacity *= 2;
    command->value = realloc(command->value, command->capacity * sizeof(char));
  }

  uint32_t char_index = command->used;
  while (char_index > position) {
    command->value[char_index] = command->value[char_index - 1];
    char_index -= 1;
  }

  command->value[position] = c;
  command->used += 1;
}

void command_remove_char(Command *command, uint8_t target) { 
  for (int i = target - 1; i < command->used; i++) {
    command->value[i] = command->value[i + 1];
  }

  command->value[command->used] = '\0';
  command->used -= 1;
}

void command_print_command(Command *command) {
  printf("\n COMMAND: ");
  for (uint8_t i = 0; i < command->used; i++) {
    printf("%c", command->value[i]);
  }
  printf("\n");
}
