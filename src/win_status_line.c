#include <ncurses.h>
#include "win_status_line.h"

WINDOW* window_status_line = NULL;

WINDOW* create_status_line_win(int pos_x, int pos_y) {
  WINDOW *win = newwin(1, pos_x, pos_y, 0);
  if (win == NULL) {
    /* TODO: Handle */
  }
  keypad(win, TRUE);
  return win;
}

// TODO: cursor_x && cursor_y is not needed here
void update_status_line(const char* filename, int window_size_x, char* mode, int cursor_x, int cursor_y) {
  wmove(window_status_line, 0, 0);
  wclrtoeol(window_status_line);

  mvwprintw(window_status_line, 0, 0, "%s", filename);
  mvwprintw(window_status_line, 0, window_size_x - 20, "%s %2d,%2d", mode, cursor_x, cursor_y);

  wrefresh(window_status_line);
}
