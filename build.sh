#!/bin/bash

gcc src/led.c src/args.c src/buffer.c src/colors.c src/command.c src/files.c src/line.c src/mode.c src/win_buffer.c src/win_line_number.c src/win_status_line.c -o ./bin/led -v -l ncurses
