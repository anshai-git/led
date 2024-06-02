#include "mode.h"

char *mode_to_string(Editor_Mode mode) {
  switch (mode) {

  case NORMAL: {
    return "NORMAL";
  } break;

  case INSERT: {
    return "INSERT";
  } break;

  default: return "";

  }
}
