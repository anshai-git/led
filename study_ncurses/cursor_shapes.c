#include <ncurses.h>

int main(int argc, char **argv) {

  initscr();
  start_color();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);

  WINDOW* win = newwin(30, 30, 0, 0);
  wrefresh(win);
  wprintw(win, "TEST TEST TEST");
  wrefresh(win);

  refresh();

  getch();
  endwin();
  return 0;
}
