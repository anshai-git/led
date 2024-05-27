#include <ncurses.h>

#include "args.h"
#include "files.h"
#include "colors.h"

void init_ncurses();

/* This should be somehow dynamic in the future */
#define LINE_NUMBER_WINDOW_WIDTH 5

int main(int argc, char **argv) {

  const char *filename = parse_filename(argc, argv);
  if (NULL == filename) return 1;

  FILE *file = read_file(filename);
  if (NULL == file) return 1;

  init_ncurses();
  init_custom_colors();
  init_color_pairs();

  int cursor_x = 0,
      cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int MAX_X = 0,
      MAX_Y = 0;
  getmaxyx(stdscr, MAX_Y, MAX_X);

  WINDOW *w_content = newwin(MAX_Y, MAX_X, 0, LINE_NUMBER_WINDOW_WIDTH);
  WINDOW *p_content = subpad(w_content, 0, 0, 0, 0);

  keypad(w_content, TRUE);
  keypad(stdscr, TRUE);
  scrollok(w_content, TRUE);

  // char buf[128];
  // int line = 1;
  // while (fgets(buf, sizeof(buf), file) != NULL) {
  //   mvwprintw(p_content, cursor_y, cursor_x, "%s", buf);
  //   cursor_y++;
  // }

  // - print file content into buffer
  // wrefresh(w_content);
  // prefresh(p_content, 0, 0, 0, 0, 30, 30);


  int input;
  while ((input = getch()) != KEY_F(1)) {
    pechochar(p_content, input);
    refresh();
  }

  getch();
  endwin();

  fclose(file);
  return 0;
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();

  start_color();
}
