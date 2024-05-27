#include <cstring>
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10

int start_x = 0;
int start_y = 0;

const char *choices[] = {
    "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight);
void report_choice(int mouse_x, int mouse_y, int *p_choice);

int main(int argc, char **argv) {
  int c, choice = 0;
  WINDOW *menu_win;
  MEVENT event;

  initscr();
  clear();
  noecho();
  cbreak();

  start_x = (80 - WIDTH) / 2;
  start_y = (24 - HEIGHT) / 2;

  attron(A_REVERSE);
  mvprintw(23, 1, "Click on exit to quit");
  refresh();
  attroff(A_REVERSE);

  menu_win = newwin(HEIGHT, WIDTH, start_x, start_y);
  print_menu(menu_win, 1);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  while(1) {
    c = wgetch(menu_win);
    attron(A_REVERSE);
    mvprintw(20, 0, "SOMETHING HAPPEND: '%c' ", c);
    refresh();
    attroff(A_REVERSE);
    switch(c) {
      case KEY_MOUSE:
        attron(A_REVERSE);
        mvprintw(23, 0, "SOMETHING HAPPEND");
        refresh();
        attroff(A_REVERSE);
        mvprintw(23, 0, "SOMETHING HAPPEND %d", event.bstate);
        if (getmouse(&event) == OK) {
          if (event.bstate & BUTTON1_PRESSED) {
            attron(A_REVERSE);
            mvprintw(23, 0, "SOMETHING PRESSED");
            refresh();
            attroff(A_REVERSE);
            report_choice(event.x + 1, event.y + 1, &choice);
            if (choice == -1) goto end;
            mvprintw(22, 1, "Choice made is : %d, String Chosen is \"%10s\"", choice, choices[choice - 1]);
            refresh();
          }
        }
        print_menu(menu_win, choice);
        break;
    }
  }
end:
  endwin();
  return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
  int x = 2, y = 2, i;

  box(menu_win, 0, 0);
  for (i = 0; i < n_choices; ++i) {
    if (highlight == i + 1) {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, A_REVERSE);
    } else {
      mvwprintw(menu_win, y, x, "%s", choices[i]);
    }
    ++y;
  }
  wrefresh(menu_win);
}

void report_choice(int mouse_x, int mouse_y, int *p_choice) {
  int i = start_x + 2, j = start_y + 3, choice;

  for (choice = 0; choice < n_choices; ++choice) {
    if (mouse_y == j + choice && mouse_x >= i &&
        mouse_x <= i + strlen(choices[choice])) {
      if (choice == n_choices - 1) {
        *p_choice = -1;
      } else {
        *p_choice = choice + 1;
      }
    }
  }
}
