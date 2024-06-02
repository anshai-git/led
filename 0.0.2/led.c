#include <ncurses.h>
#include <stdio.h>
#include <wchar.h>

#include "args.h"
#include "buffer.h"
#include "colors.h"
#include "files.h"
#include "line.h"
#include "mode.h"
#include "w_content.h"

#define CTRL(c) (c & 037)

void init_ncurses();

void init_ncurses_buffer_win(WINDOW *win);

void handle_input_normal_mode(Buffer *buffer, WINDOW *win, int input, int *cursor_y, int *cursor_x, Editor_Mode *mode);

void handle_input_insert_mode(Buffer *buffer, WINDOW *win, int input, int *cursor_y, int *cursor_x, Editor_Mode *mode);

void enter_command_mode();

void handle_edit_actions(Buffer *buffer, WINDOW *w_content, int *cursor_y, int *cursor_x, int input);

void handle_file_actions(WINDOW *window, Buffer *buffer, FILE *file, int input);

void log_line(FILE *log_file, Buffer *buffer, int line_number);

int main(int argc, char **argv) {
  const char *filename = parse_filename(argc, argv);
  if (NULL == filename) return 1;

  FILE *logs = fopen("logs.txt", "w");
  if (NULL == logs) return 1;

  FILE *file = open_or_create_new_file(filename);
  if (NULL == file) return 1;

  init_ncurses();
  init_custom_colors();
  init_color_pairs();

  int cursor_x = 0, cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int MAX_X = 0, MAX_Y = 0;
  getmaxyx(stdscr, MAX_Y, MAX_X);

  Editor_Mode MODE = NORMAL;

  WINDOW *w_content = create_buffer_window(MAX_Y - 1, MAX_X, 0, 0);
  // set_cursor_shape(w_content, STEADY_BAR);

  WINDOW *w_status_line = newwin(1, MAX_X, MAX_Y - 1, 0);

  mvwprintw(w_status_line, 0, 0, "%s", filename);
  mvwprintw(w_status_line, 0, MAX_X - 20, "%s %2d,%2d", mode_to_string(MODE), cursor_x, cursor_y);

  int W_CONTENT_MAX_X = 0, W_CONTENT_MAX_Y = 0;
  getmaxyx(w_content, W_CONTENT_MAX_Y, W_CONTENT_MAX_X);

  Buffer buffer;
  init_buffer(&buffer);

  Line line;
  init_line(&line);
  add_line(&buffer, line);

  wrefresh(w_status_line);
  wrefresh(w_content);

  wmove(w_content, cursor_y, cursor_x);
  int input;

  while ((input = wgetch(w_content)) != KEY_F(1)) {
    switch (MODE) {
    case NORMAL:
      handle_input_normal_mode(&buffer, w_content, input, &cursor_y, &cursor_x, &MODE);
      break;
    case INSERT:
      handle_input_insert_mode(&buffer, w_content, input, &cursor_y, &cursor_x, &MODE);
      break;
    case COMMAND:
      enter_command_mode();
      break;
    default:
      break;
    }

    /* Handle different kind of actions based on MODES? */
    // handle_edit_actions(&buffer, w_content, &cursor_y, &cursor_x, input);

    // handle_file_actions(w_content, &buffer, file, input);

    wmove(w_status_line, 0, 0);
    wclrtoeol(w_status_line);

    mvwprintw(w_status_line, 0, 0, "%s", filename);
    mvwprintw(w_status_line, 0, MAX_X - 20, "%s %2d,%2d", mode_to_string(MODE), cursor_x, cursor_y);

    wrefresh(w_status_line);
    wrefresh(w_content);
  }

  free_buffer(&buffer);

  getch();
  endwin();

  fclose(file);
  return 0;
}

void handle_input_normal_mode(Buffer *buffer, WINDOW *win, int input,
                              int *cursor_y, int *cursor_x, Editor_Mode *mode) {
  switch (input) {
  case KEY_UP:
  case 'k': {
    /* Only move if there is a line that we can move to */
    if (*cursor_y - 1 >= 0) {
      /*
       * If the cursor is at a position that is ahead of the line above
       * we want to move the cursor at the end of that line.
       * */
      int top_line_length = buffer_get_line_length(buffer, *cursor_y - 1);

      *cursor_y -= 1;
      *cursor_x = top_line_length < *cursor_x ? top_line_length : *cursor_x;
    }

  } break;

  case KEY_DOWN:
  case 'j': {
    /* Only move if there is a line that we can move to */
    if (*cursor_y + 1 <= buffer_get_line_count(buffer) - 1) {
      /*
       * If the cursor is at a position that is ahead of the line below
       * we want to move the cursor at the end of that line.
       * */
      int bottom_line_length = buffer_get_line_length(buffer, *cursor_y + 1);

      *cursor_y += 1;
      *cursor_x =
          bottom_line_length < *cursor_x ? bottom_line_length : *cursor_x;
    }
  } break;

  case KEY_LEFT:
  case 'h': {
    if (*cursor_x - 1 >= 0) {
      *cursor_x -= 1;
    }
  } break;

  case KEY_RIGHT:
  case 'l': {
    if (*cursor_x + 1 <= buffer_get_line_length(buffer, *cursor_y)) {
      *cursor_x += 1;
    }
  } break;

  case 'i': {
    *mode = INSERT;
  } break;

  case ':': {
    *mode = COMMAND;
  } break;

  default:
    break;
  }

  wmove(win, *cursor_y, *cursor_x);
}

void handle_input_insert_mode(Buffer *buffer, WINDOW *win, int input,
                              int *cursor_y, int *cursor_x, Editor_Mode *mode) {
  /* Check for ESC key
   * NOTE: 27 can be either ESC or ALT
   * */
  if (27 == input) {
    *mode = NORMAL;
    return;
  }

  handle_edit_actions(buffer, win, cursor_y, cursor_x, input);
  wmove(win, *cursor_y, *cursor_x);
}

void enter_command_mode(WINDOW* status_line) {
  wmove(status_line, 0, 0);
  wclrtoeol(status_line);
  waddch(status_line, ':');

  int input = wgetch(status_line);
  while(input != KEY_ENTER &&
        input != 10 /* Enter */ &&
        input != 27 /* Esc / Alt */) {
    waddch(status_line, input);
    input = wgetch(status_line);
  }
}

void handle_edit_actions(Buffer *buffer, WINDOW *w_content, int *cursor_y,
                         int *cursor_x, int input) {
  switch (input) {
  case KEY_UP: {
    /* Only move if there is a line that we can move to */
    if (*cursor_y - 1 >= 0) {
      /*
       * If the cursor is at a position that is ahead of the line above
       * we want to move the cursor at the end of that line.
       * */
      int top_line_length = buffer_get_line_length(buffer, *cursor_y - 1);

      *cursor_y -= 1;
      *cursor_x = top_line_length < *cursor_x ? top_line_length : *cursor_x;
    }

  } break;

  case KEY_DOWN: {
    /* Only move if there is a line that we can move to */
    if (*cursor_y + 1 <= buffer_get_line_count(buffer) - 1) {
      /*
       * If the cursor is at a position that is ahead of the line below
       * we want to move the cursor at the end of that line.
       * */
      int bottom_line_length = buffer_get_line_length(buffer, *cursor_y + 1);

      *cursor_y += 1;
      *cursor_x =
          bottom_line_length < *cursor_x ? bottom_line_length : *cursor_x;
    }
  } break;

  case KEY_LEFT: {
    if (*cursor_x - 1 >= 0) {
      *cursor_x -= 1;
    }
  } break;

  case KEY_RIGHT: {
    if (*cursor_x + 1 <= buffer_get_line_length(buffer, *cursor_y)) {
      *cursor_x += 1;
    }
  } break;

  case KEY_BACKSPACE:
  case KEY_DC:
  case 127: {
    if (*cursor_x - 1 < 0) {
      /* Cannot delete the 0'th character of the first line */
      if (*cursor_y - 1 < 0)
        break;

      /* After joining the the two lines we need to place the cursor
       * in the 'middle' so we save the length of the top line */
      int top_line_length = buffer_get_line_length(buffer, *cursor_y - 1);

      /* If we are at the begining of a line and want to delete:
       * append the current line to the line above*/
      join_lines(buffer, *cursor_y, *cursor_y - 1);
      remove_line(buffer, *cursor_y);

      *cursor_y -= 1;
      *cursor_x = top_line_length;

      wclrtobot(w_content);

      /* In case a whole line is deleted all the lines below that
       * are shifted so we need to redraw all the lines below */
      for (int i = *cursor_y; i < buffer_get_line_count(buffer); i++) {
        w_reprint_line(w_content, buffer, i);
      }
    } else {
      /* If we are anywhere in the line we just delete one character
       * and move the cusrsor back one position */
      buffer_del_ch(buffer, input, *cursor_y, *cursor_x);
      *cursor_x -= 1;

      wmove(w_content, *cursor_y, *cursor_x);
      wclrtoeol(w_content);

      /* In case of a single character removal on a line we only
       * need to redraw that one specific line */
      w_reprint_line(w_content, buffer, *cursor_y);
    }

  } break;

  case KEY_ENTER:
  case 10: {
    Line line;
    init_line(&line);
    split_line(*cursor_x, &buffer->lines[*cursor_y], &line);
    insert_line(buffer, line, *cursor_y);

    wclrtobot(w_content);
    for (int i = *cursor_y; i < buffer_get_line_count(buffer); i++) {
      w_reprint_line(w_content, buffer, i);
    }

    *cursor_y += 1;
    *cursor_x = 0;

    w_reprint_line(w_content, buffer, *cursor_y);
  } break;

  default: {
    /* If we make it into the default case
     * it means that we have to insert the character */

    /* Update Buffer */
    buffer_add_ch(buffer, input, *cursor_y, *cursor_x);

    /* Update Screen */
    w_reprint_line(w_content, buffer, *cursor_y);

    /* update current position */
    *cursor_x += 1;
  } break;
  }
}

void handle_file_actions(WINDOW *window, Buffer *buffer, FILE *file, int input) {
  switch (input) {
  case CTRL('s'): {
    waddch(window, input + '0');
  } break;
  default:
    break;
  }
  wrefresh(window);
}

void log_line(FILE *log_file, Buffer *buffer, int line_number) {
  Line *line = &buffer->lines[line_number];
  fprintf(log_file, "[C]: %d, [U]: %d, [LINE]: %s\n", line->capacity, line->used, line->value);
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
}
