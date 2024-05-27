#include <ncurses.h>

typedef struct _win_border_struct {
  chtype ls, rs, ts, bs, tl, tr, bl, br;
} WIN_BORDER;

typedef struct _WIN_struct {
  int start_x, start_y, height, width;
  WIN_BORDER border;
} WIN;

void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);

int main(int argc, char **argv) {
  WIN win;
  int ch;

  initscr();
  start_color();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_win_params(&win);
  print_win_params(&win);

  attron(COLOR_PAIR(1));
  printw("PRESS F1 TO EXIT");
  refresh();
  attroff(COLOR_PAIR(1));

  create_box(&win, TRUE);

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case KEY_LEFT:
      create_box(&win, FALSE);
      --win.start_x;
      create_box(&win, TRUE);
      break;
    case KEY_RIGHT:
      create_box(&win, FALSE);
      ++win.start_x;
      create_box(&win, TRUE);
      break;
    case KEY_UP:
      create_box(&win, FALSE);
      --win.start_y;
      create_box(&win, TRUE);
      break;
    case KEY_DOWN:
      create_box(&win, FALSE);
      ++win.start_y;
      create_box(&win, TRUE);
      break;
    }
  }

  endwin();
  return 0;
}

void init_win_params(WIN *win) {
  win->height = 3;
  win->width = 10;
  win->start_y = (LINES - win->height) / 2;
  win->start_x = (COLS - win->width) / 2;

  win->border.ls = '|';
  win->border.rs = '|';
  win->border.ts = '_';
  win->border.bs = '_';

  win->border.tl = '+';
  win->border.tr = '+';
  win->border.bl = '+';
  win->border.br = '+';
}

void print_win_params(WIN *p_win) {
#ifdef _DEBUG
  mvprintw(25, 0, "%d %d %d %d", win->start_x, win->start_y, win->width,
           win->height);
  refresh();
#endif
}

void create_box(WIN *win, bool flag) {
  int i, j, x, y, h, w;

  x = win->start_x;
  y = win->start_y;
  w = win->width;
  h = win->height;

  if (flag == TRUE) {
    mvaddch(y, x, win->border.tl);
    mvaddch(y, x + w, win->border.tr);
    mvaddch(y + h, x, win->border.bl);
    mvaddch(y + h, x + w, win->border.br);
    mvhline(y, x + 1, win->border.ts, w - 1);
    mvhline(y + h, x + 1, win->border.bs, w - 1);
    mvvline(y + 1, x, win->border.ls, h - 1);
    mvvline(y + 1, x + w, win->border.rs, h - 1);
  } else {
    for (j = y; j <= y + h; ++j) {
      for (i = x; i <= x + w; ++i) {
        mvaddch(j, i, ' ');
      }
    }
  }

  refresh();
}
