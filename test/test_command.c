#include <stdio.h>
#include "command.h"

int main(int argc, char** argv) {
  Command* command = create_command();
  command_insert_char(command, 'a', 0); // a
  command_insert_char(command, 'b', 1); // ab
  command_insert_char(command, 'c', 2); // abc
  command_insert_char(command, 'd', 0); // dabc
  command_insert_char(command, 'e', 1); // deabc
  command_print_command(command);
  return 0;
}
