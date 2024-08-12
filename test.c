#include <stdio.h>
#include "command.h"

int main(int argc, char** argv) {
  Command* command = create_command();
  command_insert_char(command, 'a', 0);
  command_insert_char(command, 'b', 0);
  command_print_command(command);
  return 0;
}
