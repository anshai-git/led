# A personal project with the intention of learning some stuff while writing a text editor in c/cpp

# TODO:
[ ] - Support different 'modes' like:
      - Normal
      - Insert
      - Select
[ ] - Change cursor shapes between modes

# Notes:

## Cursor types:
- echo -e -n "\x1b[\x30 q" # changes to blinking block

- echo -e -n "\x1b[\x31 q" # changes to blinking block also

- echo -e -n "\x1b[\x32 q" # changes to steady block

- echo -e -n "\x1b[\x33 q" # changes to blinking underline

- echo -e -n "\x1b[\x34 q" # changes to steady underline

- echo -e -n "\x1b[\x35 q" # changes to blinking bar

- echo -e -n "\x1b[\x36 q" # changes to steady bar


source: https://superuser.com/questions/607478/how-do-you-change-the-xterm-cursor-to-an-i-beam-or-vertical-bar/607479#607479
