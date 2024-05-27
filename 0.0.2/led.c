#include <ncurses.h>
#include <stdio.h>

#include "args.h"
#include "buffer.h"
#include "files.h"
#include "colors.h"
#include "line.h"

void init_ncurses();

int main(int argc, char **argv) {
  const char *filename = parse_filename(argc, argv);
  if (NULL == filename) return 1;

  FILE *file = read_file(filename);
  if (NULL == file) return 1;

  init_ncurses();
  init_custom_colors();
  init_color_pairs();

  int cursor_x = 0,
      cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int MAX_X = 0,
      MAX_Y = 0;
  getmaxyx(stdscr, MAX_Y, MAX_X);

  
  WINDOW *w_content = newwin(MAX_Y, MAX_X, 0, 0);
  refresh();

  mvwprintw(w_content, 1, 1, "HELLO TEXT");

  keypad(w_content, TRUE);
  keypad(stdscr, TRUE);

  char c;
  int line_number = 1;

  Buffer buffer;
  init_buffer(&buffer);
  // print_buffer(&buffer);
  
  Line line;
  init_line(&line);
  while ((c = fgetc(file)) != EOF) {
    add_char(&line, c);
    if (c == '\n') {
      add_line(&buffer, line);
      init_line(&line);
    }
  }

  /* Load buffer into window */
  for (int i = 0; i < buffer.used; i++) {
    mvwprintw(w_content, i, cursor_x, "%s", buffer.lines[i]);
  }
  wrefresh(w_content);

  getch();
  endwin();

  fclose(file);
  return 0;
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();

  start_color();
}
