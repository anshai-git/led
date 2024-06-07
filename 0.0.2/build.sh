#!/bin/bash

gcc led.c args.c args.h buffer.c buffer.h colors.c colors.h command.c command.h files.c files.h line.c line.h mode.c mode.h w_content.c w_content.h w_line_number.c w_line_number.h w_status_line.c w_status_line.h -o led -l ncurses
