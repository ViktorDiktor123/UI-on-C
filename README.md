# UI-from-C

# dependencies
All libraries are connected to UI.h
And only standard language C libraries are used 

# using the library
1) connect the library via #include "UI.h"
2) pixel(uint8_t x, uint8_t y); - adding a pixel by coordinates
3) rect(uint8_t rect[4], bool full); - adding a rectangle to the screen (x, y, weight, height)
4) line(uint8_t line[4]); - adding a line to the screen (start x, start y, end x, end y)
5) circle(uint8_t c[3], bool full); - adding a circle to the screen (x, y, radius)
6) font(char file[]); - loading font
7) text(char text_str[], uint8_t x, uint8_t y); - outputting text to the screen
8) key(); - returns the key symbol from the keyboard
9) mouse(uint8_t m[3]) - m[0] - key, m[1] - X, m[2] - Y, returns successful events
10) colliderect(uint8_t r1[4], uint8_t r2[4]); - collide rectangles
11) collidepoint(const uint8_t r[4], uint8_t px, uint8_t py); - collide point & rect
12) collideline(uint8_t rect[4], uint8_t line[4]) - collide line & rect
13) collidecircle(uint8_t c[3], uint8_t r[4]) - collide circle & rect
14) draw(); - screen rendering
15) clear(); - clear screen data

# peculiarities
1) screen 30x30
2) the screen array takes up 120 bytes
3) font symbols: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()!?.,;:-+*/ '
4) the font takes up exactly 2 KB

# test code
You can view the test code test.c
or compile it and run it
