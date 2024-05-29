#include <ncurses.h>
#include <stdio.h>
#include <wchar.h>

#include "args.h"
#include "buffer.h"
#include "colors.h"
#include "files.h"
#include "line.h"
#include "w_content.h"

void init_ncurses();
void init_ncurses_buffer_win(WINDOW* win);
void log_line(FILE *log_file, Buffer *buffer, int line_number);

/*
 * TODO:
 * [ ] - IN BUFFER: Do lines contain newlines at the end?
 * [ ] - Create new file if file from argument doesnt exist
 * */

int main(int argc, char **argv) {
  const char *filename = parse_filename(argc, argv);
  if (NULL == filename)
    return 1;

  FILE *logs = fopen("logs.txt", "w");
  if (NULL == logs)
    return 1;

  FILE *file = read_file(filename);
  if (NULL == file)
    return 1;

  init_ncurses();
  init_custom_colors();
  init_color_pairs();

  int cursor_x = 0, cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int MAX_X = 0, MAX_Y = 0;
  getmaxyx(stdscr, MAX_Y, MAX_X);

  WINDOW *w_content = create_buffer_window(MAX_Y, MAX_X, 0, 0);
  set_cursor_shape(w_content, STEADY_BAR);

  int W_CONTENT_MAX_X = 0, W_CONTENT_MAX_Y = 0;
  getmaxyx(w_content, W_CONTENT_MAX_Y, W_CONTENT_MAX_X);

  refresh();

  Buffer buffer;
  init_buffer(&buffer);

  Line line;
  init_line(&line);
  add_line(&buffer, line);

  /* TODO: Create file if it doesnt exist */
  // load_file(&buffer, file);
  // w_load_buffer(w_content, &buffer);

  int input;
  while ((input = wgetch(w_content)) != KEY_F(1)) {
    switch (input) {
    case KEY_UP: {
      /* Only move if there is a line that we can move to */
      if (cursor_y - 1 >= 0) {
        /*
         * If the cursor is at a position that is ahead of the line above
         * we want to move the cursor at the end of that line.
         * */
        int top_line_length = buffer.lines[cursor_y - 1].used;

        cursor_y--;
        cursor_x = top_line_length < cursor_x ? top_line_length : cursor_x;

        wmove(w_content, cursor_y, cursor_x);
      }

    } break;

    case KEY_DOWN: {
      /* Only move if there is a line that we can move to */
      if (cursor_y + 1 <= (buffer.used - 1)) {
        /*
         * If the cursor is at a position that is ahead of the line below
         * we want to move the cursor at the end of that line.
         * */
        int bottom_line_length = buffer.lines[cursor_y + 1].used;

        cursor_y++;
        cursor_x =
            bottom_line_length < cursor_x ? bottom_line_length : cursor_x;

        wmove(w_content, cursor_y, cursor_x);
      }
    } break;

    case KEY_LEFT: {
      if (cursor_x - 1 >= 0) {
        cursor_x--;
        wmove(w_content, cursor_y, cursor_x);
      }
    } break;

    case KEY_RIGHT: {
      if (cursor_x + 1 <= buffer.lines[cursor_y].used) {
        cursor_x++;
        wmove(w_content, cursor_y, cursor_x);
      }
    } break;

    case KEY_BACKSPACE:
    case KEY_DC:
    case 127: {
      if (cursor_x - 1 < 0) {
        /* Cannot delete the 0'th character of the first line */
        if (cursor_y - 1 < 0)
          break;

        /* After joining the the two lines we need to place the cursor
         * in the 'middle' so we save the length of the top line */
        int top_line_length = buffer.lines[cursor_y - 1].used;

        /* If we are at the begining of a line and want to delete:
         * append the current line to the line above*/
        join_lines(&buffer, cursor_y, cursor_y - 1);
        remove_line(&buffer, cursor_y);

        cursor_y--;
        cursor_x = top_line_length;

        wclrtobot(w_content);

        /* In case a whole line is deleted all the lines below that
         * are shifted so we need to redraw all the lines below */
        for (int i = cursor_y; i < buffer.used; i++) {
          w_reprint_line(w_content, &buffer, i);
        }
        wmove(w_content, cursor_y, cursor_x);
      } else {
        /* If we are anywhere in the line we just delete one character
         * and move the cusrsor back one position */
        buffer_del_ch(&buffer, input, cursor_y, cursor_x);
        cursor_x--;

        wmove(w_content, cursor_y, cursor_x);
        wclrtoeol(w_content);

        /* In case of a single character removal on a line we only
         * need to redraw that one specific line */
        w_reprint_line(w_content, &buffer, cursor_y);
        wmove(w_content, cursor_y, cursor_x);
      }

    } break;

    case 10: { // ENTER
      Line line;
      init_line(&line);
      split_line(cursor_x, &buffer.lines[cursor_y], &line);
      insert_line(&buffer, line, cursor_y);

      wclrtobot(w_content);
      for (int i = cursor_y; i < buffer.used; i++) {
        w_reprint_line(w_content, &buffer, i);
      }

      cursor_y += 1;
      cursor_x = 0;

      w_reprint_line(w_content, &buffer, cursor_y);

      wmove(w_content, cursor_y, cursor_x);
    } break;

    default: {
      /* If we make it into the default case
       * it means that we have to insert the character */

      /* Update Buffer */
      buffer_add_ch(&buffer, input, cursor_y, cursor_x);

      /* Update Screen */
      w_reprint_line(w_content, &buffer, cursor_y);

      /* update current position */
      cursor_x++;
      wmove(w_content, cursor_y, cursor_x);
    } break;
    }

    log_line(logs, &buffer, cursor_y);
    wrefresh(w_content);
  }

  free_buffer(&buffer);

  getch();
  endwin();

  fclose(file);
  return 0;
}


void log_line(FILE *log_file, Buffer *buffer, int line_number) {
  Line *line = &buffer->lines[line_number];
  fprintf(log_file, "[C]: %d, [U]: %d, [LINE]: %s\n", line->capacity,
          line->used, line->value);
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
}
