#ifndef _MODE_H_
#define _MODE_H_

typedef enum { 
  INSERT = 0,
  NORMAL,
  COMMAND
} Editor_Mode;

char* mode_to_string(Editor_Mode mode);

#endif
