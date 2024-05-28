#ifndef _W_CONTENT_H_
#define _W_CONTENT_H_

#include <ncurses.h>
#include "buffer.h"

void load_buffer(WINDOW* win, Buffer* buffer);
void reprint_line(WINDOW* win, Buffer* buffer, int line);

#endif
