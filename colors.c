#include <ncurses.h>
#include "colors.h"

void init_custom_colors() {
  init_color(COLOR_MAIN_BACKGROUND, 26, 50, 51);
  init_color(COLOR_MAIN_BACKGROUND, 181, 176, 165);
}

void init_color_pairs() {
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
}
