#include <ncurses.h>

extern WINDOW* window_status_line;

#ifndef _H_WIN_STATUS_LINE_
#define _H_WIN_STATUS_LINE_

WINDOW* create_status_line_win(int pos_x, int pos_y);

void update_status_line(const char* filename, int window_size_x, char* mode, int cursor_x, int cursor_y);

#endif
