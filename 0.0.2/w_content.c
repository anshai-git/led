#include <ncurses.h>
#include "buffer.h"

/* Print the buffer into the window */
void load_buffer(WINDOW* win, Buffer* buffer) {
  for (int i = 0; i < buffer->used; i++) {
    mvwprintw(win, i, 0, "%s", buffer->lines[i]);
  }
  wrefresh(win);
}
