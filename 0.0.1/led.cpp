#include <cstdio>
#include <cstdlib>
#include <ncurses.h>

/* COLORS */
// #define COLOR_MAIN_BACKGROUND 8
// #define COLOR_MAIN_FOREGROUND 8

/* This should be somehow dynamic in the future */
#define LINE_NUMBER_WINDOW_WIDTH 5

int main(int argc, char **argv) {
  // - Parse arguments
  // - Open file
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\nUSAGE: led <filename>\n");
    exit(1);
  }

  char *filename = argv[1];
  FILE *file = fopen(filename, "rw");

  if (file == NULL) {
    fprintf(stderr, "Failed to open file.");
    exit(1);
  }

  // - Initialize ncurses
  initscr();
  cbreak();
  noecho();

  // - Initialize colors
  start_color();

  /* Define colors */
  // init_color(COLOR_MAIN_BACKGROUND, 26, 50, 51);
  // init_color(COLOR_MAIN_BACKGROUND, 181, 176, 165);

  /* Define color pair with custom colors */
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);

  int cursor_x = 0, cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int MAX_X, MAX_Y;
  getmaxyx(stdscr, MAX_Y, MAX_X);

  // - Create Window for line numbers
  WINDOW *w_line_number =
      newwin(MAX_Y - 1, /* -1 to let space for the status line */
             LINE_NUMBER_WINDOW_WIDTH, 0, 0);

  WINDOW *w_content =
      newwin(MAX_Y - 1, /* -1 to let space for the status line */
             MAX_X - LINE_NUMBER_WINDOW_WIDTH, 0, LINE_NUMBER_WINDOW_WIDTH);

  /* Calling it with 0s will fill the full window? */
  WINDOW* p_content = subpad(w_content, 0, 0, 0, 0);

  WINDOW *w_status_line = newwin(1, MAX_X, MAX_Y - 1, 0);

  mvwprintw(w_status_line, 0, 0, "%s", filename);
  mvwprintw(w_status_line, 0, MAX_X - 10, "%d,%d", cursor_x, cursor_y);

  keypad(w_content, TRUE);
  keypad(stdscr, TRUE);
  scrollok(w_content, TRUE);
  refresh();

  wattron(w_line_number, COLOR_PAIR(1));
  wattron(w_content, COLOR_PAIR(2));
  wattron(w_status_line, COLOR_PAIR(2));

  // wbkgd(w_content, COLOR_PAIR(1));
  wbkgd(w_line_number, COLOR_PAIR(1));
  wbkgd(w_status_line, COLOR_PAIR(2));

  // - SHOW THE LINE NUMBER WINDOW
  // wrefresh(w_status_line);
  // wrefresh(w_line_number);
  // wrefresh(w_content);
  // refresh();

  // - Load file content into buffer
  char buf[128];
  int line = 1;
  while (fgets(buf, sizeof(buf), file) != NULL && line < 30) {
    // mvwprintw(w_line_number, cursor_y, cursor_x, "%4d", line++);
    mvwprintw(p_content, cursor_y, cursor_x, "%s", buf);
    cursor_y++;
  }

  // - print file content into buffer
  wrefresh(w_status_line);
  wrefresh(w_line_number);
  wrefresh(w_content);

  prefresh(p_content, 0, 0, 0, 0, 30, 30);

  refresh();

  int input;
  while ((input = getch()) != KEY_F(1)) {
    switch (input) {
    case KEY_UP: {
      if (cursor_y - 1 >= 0) {
        wmove(w_content, --cursor_y, cursor_x);
      }
      break;
    }
    case KEY_DOWN:
      wmove(w_content, ++cursor_y, cursor_x);
      scroll(w_content);
      break;
    case KEY_LEFT:
      if (cursor_x >= 1) {
        wmove(w_content, cursor_y, --cursor_x);
      }
      break;
    case KEY_RIGHT:
      if (cursor_x <= MAX_X) {
        wmove(w_content, cursor_y, ++cursor_x);
      }
      break;
    case KEY_BACKSPACE:
      delch();
    default:
      waddch(w_content, input);
    }

    mvwprintw(w_status_line, 0, MAX_X - 10, "%d,%d", cursor_x, cursor_y);

    werase(w_line_number);
    mvwprintw(w_line_number, cursor_y, 0, "%4d ", cursor_y);

    for (int i = cursor_y; i > 0; i--) {
      mvwprintw(w_line_number, cursor_y - i, 0, "%4d", i);
    }

    // for (int i = cursor_y; i < MAX_Y; i++) {
    //   mvwprintw(w_line_number, cursor_y + i, 0, "%4d", i);
    // }

    // getbegyx(stdscr, cursor_y, cursor_x);
    //

    /* Move cursor to the content window */
    // wmove(w_content, 1, 1);

    // - Refresh screen after moving cursor
    wrefresh(w_status_line);
    wrefresh(w_line_number);
    wrefresh(w_content);
  }

  // - End ncurses
  getch();
  endwin();

  // - Close file
  fclose(file);
  return 0;
}
