#include "w_content.h"
#include "buffer.h"
#include "line.h"
#include <ncurses.h>

void w_load_buffer(WINDOW *win, Buffer *buffer) {
  for (int i = 0; i < buffer->used; i++) {
    mvwprintw(win, i, 0, "%s", buffer->lines[i]);
  }
  wrefresh(win);
}

void w_reprint_line(WINDOW *win, Buffer *buffer, int line_n) {
  mvwprintw(win, line_n, 0, "%s", buffer->lines[line_n].value);
  wrefresh(win);
}

WINDOW *create_buffer_window(int n_rows, int n_cols, int start_y, int start_x) {
  WINDOW *win = newwin(n_rows, n_cols, start_y, start_x);
  keypad(win, TRUE);
  return win;
}

/*
 * echo -e -n "\x1b[\x30 q" # changes to blinking block
 * echo -e -n "\x1b[\x31 q" # changes to blinking block also
 * echo -e -n "\x1b[\x32 q" # changes to steady block
 * echo -e -n "\x1b[\x33 q" # changes to blinking underline
 * echo -e -n "\x1b[\x34 q" # changes to steady underline
 * echo -e -n "\x1b[\x35 q" # changes to blinking bar
 * echo -e -n "\x1b[\x36 q" # changes to steady bar
 * */
void set_cursor_shape(WINDOW *win, Cursor_Shape shape) {
  wprintw(win, "\x1b[\x36 q");
  wprintw(win, "\x1b[\x36 q");
  wprintw(win, "\x1b[\x36 q");
  wprintw(win, "\x1b[\x36 q");

  // switch (shape) {
  // default: {
  //   wprintw(win, "\x1b[\x36 q");
  // } break;
  // }
}
