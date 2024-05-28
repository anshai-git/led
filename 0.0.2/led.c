#include <ncurses.h>
#include <stdio.h>

#include "args.h"
#include "buffer.h"
#include "files.h"
#include "colors.h"
#include "line.h"
#include "w_content.h"

void init_ncurses();

/*
 * TODO:
 * [ ] - IN BUFFER: Do lines contain newlines at the end?
 * [ ] - Create new file if file from argument doesnt exist
 * */

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
  
  WINDOW *w_content = newwin(MAX_Y, MAX_X, 0, 0);
  keypad(w_content, TRUE);
  keypad(stdscr, TRUE);

  int W_CONTENT_MAX_X = 0,
      W_CONTENT_MAX_Y = 0;
  getmaxyx(w_content, W_CONTENT_MAX_Y, W_CONTENT_MAX_X);

  refresh();

  Buffer buffer;
  init_buffer(&buffer);

  /* TODO: Create file if it doesnt exist */
  load_file(&buffer, file);
  load_buffer(w_content, &buffer);

  int input;
  while ((input = wgetch(w_content)) != KEY_F(1)) {
    switch (input) {
      case KEY_UP:
        if (cursor_y - 1 >= 0) {
          cursor_y--;
          wmove(w_content, cursor_y, cursor_x);
        }
        break;

      case KEY_DOWN:
        if (cursor_y - 1 <= (W_CONTENT_MAX_Y - 1)) {
          cursor_y++;
          wmove(w_content, cursor_y, cursor_x);
        }
        break;

      case KEY_LEFT:
        cursor_x--;
        wmove(w_content, cursor_y, cursor_x);
        break;

      case KEY_RIGHT:
        cursor_x++;
        wmove(w_content, cursor_y, cursor_x);
        break;

      case KEY_BACKSPACE:
      case KEY_DC:
      case 127:
        cursor_x--;
        mvwdelch(w_content, cursor_y, cursor_x);
        break;

      case 10:
        cursor_y += 1;
        cursor_x = 0;
        waddch(w_content, '\n');
        wmove(w_content, cursor_y, cursor_x);
        /* TODO: Handle newline */
        break;

      default:
        /* If we make it into the default case
         * it means that we have to insert the character */

        /* Update Buffer */
        buffer_add_ch(&buffer, input, cursor_y, cursor_x);

        /* Update Screen */
        reprint_line(w_content, &buffer, cursor_y);

        break;
    }
    wrefresh(w_content);
  }

  free_buffer(&buffer);

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
