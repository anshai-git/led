#include <curses.h>
#include <panel.h>
#include <ncurses.h>

int main(int argc, char** argv) {
  WINDOW* windows[3];
  PANEL* panels[3];

  int lines = 10,
      cols = 40,
      y = 2,
      x = 4,
      i;

  initscr();
  cbreak();
  noecho();

  windows[0] = newwin(lines, cols, y, x);
  windows[1] = newwin(lines, cols, y + 1, x + 5);
  windows[2] = newwin(lines, cols, y + 2, x + 10);

  for (i = 0; i < 3; ++i) {
    box(windows[i], 0, 0);
  }

  panels[0] = new_panel(windows[0]);
  panels[1] = new_panel(windows[1]);
  panels[2] = new_panel(windows[2]);

  update_panels();

  doupdate();

  getch();
  endwin();

  return 0;
}
