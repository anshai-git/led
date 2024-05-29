#ifndef _W_CONTENT_H_
#define _W_CONTENT_H_

#include <ncurses.h>
#include "buffer.h"

typedef enum {
  STEADY_BLOCK = 0,
  BLINKING_BLOCK,
  BLINKING_BLOCK_2,
  BLINKING_UNDERLINE,
  STEADY_UNDERLINE,
  BLINKING_BAR,
  STEADY_BAR
} Cursor_Shape;

/* Print the buffer into the window */
void w_load_buffer(WINDOW* win, Buffer* buffer);

/* Redraw a single line from the buffer into the window */
void w_reprint_line(WINDOW* win, Buffer* buffer, int line_n);

/* Create a window configured for displaying buffers */
WINDOW* create_buffer_window(int n_rows, int n_cols, int start_y, int start_x);

/* Change cursor shape */
void set_cursor_shape(WINDOW* win, Cursor_Shape shape);

#endif
