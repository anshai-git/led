#include <ncurses.h>

int main() {
  int ch;
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("Type any character to see it in bold\n");
  while (KEY_F(1) != (ch = getch())) {
    addch(ch);
    refresh();
  }
  endwin();

  return 0;
}
