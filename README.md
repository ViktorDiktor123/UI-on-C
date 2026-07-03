# UI-from-C

# dependencies
All libraries are connected to UI.h
And only standard language C libraries are used 

# using the library
1) connect the library via #include "UI.h"
2) pixel(uint8_t x, uint8_t y); - adding a pixel by coordinates
3) rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h); - adding a rectangle to the screen
4) font(char file[]); - loading font
5) text(char text_str[], uint8_t x, uint8_t y); - outputting text to the screen
6) key(); - returns the key symbol from the keyboard
7) mouse(uint8_t m[3]) - m[0] - key, m[1] - X, m[2] - Y, returns successful events
8) draw(); - screen rendering
9) clear(); - clear screen data

# peculiarities
1) screen 30x30
2) the screen array takes up 120 bytes
3) font symbols: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()!?.,;:-+*/ '
4) the font takes up exactly 2 KB

# test code
You can view the test code test.c
or compile it and run it
