#ifndef _FILES_H_
#define _FILES_H_

#include <stdio.h>

FILE* read_file(const char* path);
FILE* open_or_create_new_file(const char* path);

#endif
