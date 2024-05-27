#include <cstring>
#include <ncurses.h>
#include <panel.h>

#define N_LINES 10
#define N_COLS 40

void init_wins(WINDOW** wins, int n);
void win_show(WINDOW* win, char* label, int label_color);
void print_in_middle(WINDOW* win,
    int start_y,
    int start_x,
    int width,
    char* string,
    chtype color);

int main(int argc, char** argv) {
  WINDOW* windows[3];
  PANEL* panels[3];
  PANEL* top;
  int ch;

  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  init_pair(1, COLOR_RED,   COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE,  COLOR_BLACK);
  init_pair(4, COLOR_CYAN,  COLOR_BLACK);

  init_wins(windows, 3);

  panels[0] = new_panel(windows[0]);
  panels[1] = new_panel(windows[1]);
  panels[2] = new_panel(windows[2]);

  set_panel_userptr(panels[0], panels[1]);
  set_panel_userptr(panels[1], panels[2]);
  set_panel_userptr(panels[2], panels[0]);

  update_panels();

  attron(COLOR_PAIR(4));
  mvprintw(LINES - 2, 0, "Use tabto browse through the windows (F1 to Exit)");
  attroff(COLOR_PAIR(4));
  doupdate();

  top = panels[2];

  while((ch = getch()) != KEY_F(1)) {
    switch(ch) {
      case 9:
        top = (PANEL*) panel_userptr(top);
        top_panel(top);
        break;
    }
    update_panels();
    doupdate();
  }
  endwin();
  return 0;
}

void init_wins(WINDOW** wins, int n) {
  int x = 10, y = 2, i;
  char label[80];

  for (i = 0; i < n; ++i) {
    wins[i] = newwin(N_LINES, N_COLS, y, x);
    sprintf(label, "Window Number %d", i + 1);
    win_show(wins[i], label, i + 1);
    y += 3;
    x += 7;
  }
}

void win_show(WINDOW* win, char* label, int label_color) {
  int start_x, start_y, height, width;

  getbegyx(win, start_y, start_x);
  getmaxyx(win, height, width);

  box(win, 0, 0);
  mvwaddch(win, 2, 0, ACS_LTEE);
  mvwhline(win, 2, 1, ACS_HLINE, width - 2);
  mvwaddch(win, 2, width - 1, ACS_RTEE);

  print_in_middle(win ,1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW* win,
    int start_y,
    int start_x,
    int width,
    char* string,
    chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL) win = stdscr;
  getyx(win, y, x);
  if (start_x != 0) x = start_x;
  if (start_y != 0) y = start_y;
  if (width == 0) width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = start_x + (int) temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
