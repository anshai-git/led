#include <ncurses.h>

WINDOW *create_new_win(int height, int width, int start_y, int start_x);

int main() {
  WINDOW *main_window;
  int start_x, start_y, width, height, ch;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);

  main_window = create_new_win(30, 30, 0, 0);
  wmove(main_window, 1, 1);
  wprintw(main_window, "\x1b[\x36 q");
  
  // wrefresh(main_window);

  getch();
  endwin();
  return 0;
}

WINDOW *create_new_win(int height, int width, int start_y, int start_x) {
  WINDOW *local_win = newwin(height, width, start_y, start_x);
  wrefresh(local_win);
  return local_win;
}
