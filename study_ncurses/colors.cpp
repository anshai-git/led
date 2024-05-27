#include <cstdlib>
#include <ncurses.h>
#include <cstring>

void print_in_middle(WINDOW* win, int start_y, int start_x, int width, const char* string);

int main(int argc, char** argv) {
  initscr();
  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);

  attron(COLOR_PAIR(1));
  print_in_middle(stdscr, LINES / 2, 0, 0, "IN :: COLOR");
  attroff(COLOR_PAIR(1));
  getch();
  endwin();

  return 0;
}

void print_in_middle(WINDOW* win, int start_y, int start_x, int width, const char* string) {
  int length, x, y;
  float temp;

  if (win == NULL) {
    win = stdscr;
  }

  getyx(win, y, x);

  if (start_x != 0) {
    x = start_x;
  }

  if (start_y != 0) {
    y = start_y;
  }

  if (width == 0) {
    width = 80;
  }

  length = strlen(string);
  temp = (width - length) / 2;
  x = start_x + (int) temp;
  mvwprintw(win, y, x, "%s", string);
  refresh();
}
