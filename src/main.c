#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"
#include "buffer.h"
#include "colors.h"
#include "command.h"
#include "files.h"
#include "line.h"
#include "mode.h"
#include "win_buffer.h"
#include "win_status_line.h"

#define CTRL(c) (c & 037)

/* LOGGING > */
void log_info(const char* message) {
  FILE *log_file = fopen("logs.txt", "a");
  if (log_file != NULL) {
    fprintf(log_file, "%s", message);
  }
  fclose(log_file);
}
/* LOGGING < */

void init_ncurses();

void handle_input_normal_mode(Buffer *buffer, WINDOW *win, int input, Editor_Mode *mode);

void handle_input_insert_mode(Buffer *buffer, WINDOW *win, int input, Editor_Mode *mode);

void enter_command_mode(WINDOW* win);

void handle_edit_actions(Buffer *buffer, WINDOW *window_buffer, int input);

void handle_file_actions(WINDOW *window, Buffer *buffer, FILE *file, int input);

int main(int argc, char **argv) {
  const char *filename = args_parse_filename(argc, argv);
  if (NULL == filename)
    return 1;

  FILE *file = open_or_create_new_file(filename);
  if (NULL == file) return 1;

  init_ncurses();
  init_custom_colors();
  init_color_pairs();

  int cursor_x = 0, cursor_y = 0;
  getbegyx(stdscr, cursor_y, cursor_x);

  int WINDOW_SIZE_X = 0, WINDOW_SIZE_Y = 0;
  getmaxyx(stdscr, WINDOW_SIZE_Y, WINDOW_SIZE_X);

  /* init ncurses windows */
  window_status_line = create_status_line_win(WINDOW_SIZE_X, WINDOW_SIZE_Y - 1);
  /* this should hold the curent window or buffer object, or a wrapper around both */
  window_buffer = create_buffer_window(WINDOW_SIZE_Y - 1, WINDOW_SIZE_X, 0, 0);

  Editor_Mode MODE = NORMAL;

  Buffer *buffer = create_buffer();
  Line *line = create_line();
  buffer_append_line(buffer, *line);

  update_status_line(filename, WINDOW_SIZE_X, mode_to_string(MODE), buffer->cursor_x, buffer->cursor_y);
  wrefresh(window_buffer);
  wmove(window_buffer, buffer->cursor_y, buffer->cursor_x);

  int input;

  while ((input = wgetch(window_buffer)) != KEY_F(2)) {
    switch (MODE) {
    case NORMAL:
      handle_input_normal_mode(buffer, window_buffer, input, &MODE);
      break;
    case INSERT:
      handle_input_insert_mode(buffer, window_buffer, input, &MODE);
      break;
    default:
      break;
    }

    update_status_line(filename, WINDOW_SIZE_X, mode_to_string(MODE), buffer->cursor_x, buffer->cursor_y);
    wrefresh(window_buffer);
  }

  buffer_free(buffer);

  getch();
  endwin();

  fclose(file);
  return 0;
}

void handle_input_normal_mode(Buffer *buffer, WINDOW *win, int input, Editor_Mode *mode) {
  switch (input) {
  case KEY_UP:
  case 'k': {
    /* Only move if there is a line that we can move to */
    if (buffer->cursor_y - 1 >= 0) {
      /*
       * If the cursor is at a position that is ahead of the line above
       * we want to move the cursor at the end of that line.
       * */
      int top_line_length = buffer_get_line_length(buffer, buffer->cursor_y - 1);

      buffer->cursor_y -= 1;
      buffer->cursor_x = top_line_length < buffer->cursor_x ? top_line_length : buffer->cursor_x;
    }
  } break;

  case KEY_DOWN:
  case 'j': {
    /* Only move if there is a line that we can move to */
    if (buffer->cursor_y + 1 <= buffer_get_line_count(buffer) - 1) {
      /*
       * If the cursor is at a position that is ahead of the line below
       * we want to move the cursor at the end of that line.
       * */
      int bottom_line_length = buffer_get_line_length(buffer, buffer->cursor_y + 1);

      buffer->cursor_y += 1;
      buffer->cursor_x =
          bottom_line_length < buffer->cursor_x ? bottom_line_length : buffer->cursor_x;
    }
  } break;

  case KEY_LEFT:
  case 'h': {
    if (buffer->cursor_x - 1 >= 0) {
      buffer->cursor_x -= 1;
    }
  } break;

  case KEY_RIGHT:
  case 'l': {
    if (buffer->cursor_x + 1 <= buffer_get_line_length(buffer, buffer->cursor_y)) {
      buffer->cursor_x += 1;
    }
  } break;

  case 'i': {
    *mode = INSERT;
  } break;

  case ':': {
    enter_command_mode(window_status_line);
  } break;

  default:
    break;
  }

  wmove(win, buffer->cursor_y, buffer->cursor_x);
}

void handle_input_insert_mode(Buffer *buffer, WINDOW *win, int input, Editor_Mode *mode) {
  /* Check for ESC key
   * NOTE: 27 can be either ESC or ALT
   * */
  if (27 == input) {
    *mode = NORMAL;
    return;
  }

  handle_edit_actions(buffer, win, input);
  wmove(win, buffer->cursor_y, buffer->cursor_x);
}

void enter_command_mode(WINDOW *status_line) {
  wmove(status_line, 0, 0);
  wclrtoeol(status_line);
  waddch(status_line, ':');

  int input = 0;
  Command* command = create_command();

  uint8_t position_x = 1;
  while (input != KEY_ENTER && input != 10 /* Enter */ && input != 27 /* Esc / Alt */) {
    input = wgetch(status_line);
    switch (input) {
      case KEY_LEFT: {
        if (position_x > 1)  {
          position_x -= 1;
        }
      } break;
      case KEY_RIGHT: {
        if (position_x < command->used + 1)  {
          position_x += 1;
        }
      } break;
      case KEY_BACKSPACE:
      case KEY_DC:
      case 127: {
        if (position_x > 1) {
          command_remove_char(command, position_x - 1);
          position_x -= 1;
        } // TODO: else > invalid command
      } break;

      default: {
        command_insert_char(command, input, position_x - 1);
        position_x += 1;
      } break;
    }

    wmove(status_line, 0, 1);
    wclrtoeol(status_line);

    mvwprintw(status_line, 0, 1, command->value);
    wmove(status_line, 0, position_x);
  }
}

void handle_edit_actions(Buffer *buffer, WINDOW *window_buffer, int input) {
  switch (input) {
  case KEY_UP: {
    /* Only move if there is a line that we can move to */
    if (buffer->cursor_y - 1 >= 0) {
      /*
       * If the cursor is at a position that is ahead of the line above
       * we want to move the cursor at the end of that line.
       * */
      int top_line_length = buffer_get_line_length(buffer, buffer->cursor_y - 1);

      buffer->cursor_y -= 1;
      buffer->cursor_x = top_line_length < buffer->cursor_x ? top_line_length : buffer->cursor_x;
    }
  } break;

  case KEY_DOWN: {
    /* Only move if there is a line that we can move to */
    if (buffer->cursor_y + 1 <= buffer_get_line_count(buffer) - 1) {
      /*
       * If the cursor is at a position that is ahead of the line below
       * we want to move the cursor at the end of that line.
       * */
      int bottom_line_length = buffer_get_line_length(buffer, buffer->cursor_y + 1);

      buffer->cursor_y += 1;
      buffer->cursor_x =
          bottom_line_length < buffer->cursor_x ? bottom_line_length : buffer->cursor_x;
    }
  } break;

  case KEY_LEFT: {
    if (buffer->cursor_x - 1 >= 0) {
      buffer->cursor_x -= 1;
    }
  } break;

  case KEY_RIGHT: {
    if (buffer->cursor_x + 1 <= buffer_get_line_length(buffer, buffer->cursor_y)) {
      buffer->cursor_x += 1;
    }
  } break;

  case KEY_BACKSPACE:
  case KEY_DC:
  case 127: {
    if (buffer->cursor_x - 1 < 0) {
      /* Cannot delete the 0'th character of the first line */
      if (buffer->cursor_y - 1 < 0) break;

      /* After joining the the two lines we need to place the cursor
       * in the 'middle' so we save the length of the top line */
      int top_line_length = buffer_get_line_length(buffer, buffer->cursor_y - 1);

      /* If we are at the begining of a line and want to delete:
       * append the current line to the line above*/
      buffer_join_lines(buffer, buffer->cursor_y, buffer->cursor_y - 1);
      buffer_remove_line(buffer, buffer->cursor_y);

      buffer->cursor_y -= 1;
      buffer->cursor_x = top_line_length;

      wclrtobot(window_buffer);

      /* In case a whole line is deleted all the lines below that
       * are shifted so we need to redraw all the lines below */
      for (int i = buffer->cursor_y; i < buffer_get_line_count(buffer); i++) {
        w_reprint_line(window_buffer, buffer, i);
      }
    } else {
      /* If we are anywhere in the line we just delete one character
       * and move the cusrsor back one position */
      buffer_delete_ch(buffer, buffer->cursor_y, buffer->cursor_x);
      buffer->cursor_x -= 1;

      wmove(window_buffer, buffer->cursor_y, buffer->cursor_x);
      wclrtoeol(window_buffer);

      /* In case of a single character removal on a line we only
       * need to redraw that one specific line */
      w_reprint_line(window_buffer, buffer, buffer->cursor_y);
    }
  } break;

  case KEY_ENTER:
  case 10: {
    Line *line = create_line();
    line_split_line(buffer->cursor_x, &buffer->lines[buffer->cursor_y], line);
    buffer_insert_line(buffer, *line, buffer->cursor_y);

    wclrtobot(window_buffer);
    for (int i = buffer->cursor_y; i < buffer_get_line_count(buffer); i++) {
      w_reprint_line(window_buffer, buffer, i);
    }

    buffer->cursor_y += 1;
    buffer->cursor_x = 0;

    w_reprint_line(window_buffer, buffer, buffer->cursor_y);
  } break;

  default: {
    /* If we make it into the default case
     * it means that we have to insert the character */

    /* Update Buffer */
    buffer_insert_ch(buffer, input, buffer->cursor_y, buffer->cursor_x);

    /* Update Screen */
    w_reprint_line(window_buffer, buffer, buffer->cursor_y);

    /* update current position */
    buffer->cursor_x += 1;
  } break;
  }
}

void handle_file_actions(WINDOW *window, Buffer *buffer, FILE *file, int input) {
  switch (input) {
  case CTRL('s'): {
    // waddch(window, input + '0');
  } break;
  default:
    break;
  }
  wrefresh(window);
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
}
