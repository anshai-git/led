#include <ncurses.h>

WINDOW *create_new_win(int height, int width, int start_y, int start_x);
void destroy_win(WINDOW *win);

int main() {
  WINDOW *main_window;
  int start_x, start_y, width, height, ch;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);

  height = 3;
  width = 10;

  start_y = (LINES - height) / 2;
  start_x = (COLS - width) / 2;

  printw("PRESS F1 TO EXIT");
  refresh();

  main_window = create_new_win(height, width, start_y, start_x);

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
      case KEY_LEFT: 
        destroy_win(main_window);
        main_window = create_new_win(height, width, start_y, --start_x);
        break;
      case KEY_RIGHT: 
        destroy_win(main_window);
        main_window = create_new_win(height, width, start_y, ++start_x);
        break;
      case KEY_UP: 
        destroy_win(main_window);
        main_window = create_new_win(height, width, --start_y, start_x);
        break;
      case KEY_DOWN: 
        destroy_win(main_window);
        main_window = create_new_win(height, width, ++start_y, start_x);
        break;
    }
  }

  endwin();
  return 0;
}

WINDOW *create_new_win(int height, int width, int start_y, int start_x) {
  WINDOW *local_win = newwin(height, width, start_y, start_x);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return local_win;
}

void destroy_win(WINDOW *win) {
  wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(win);
  delwin(win);
}

