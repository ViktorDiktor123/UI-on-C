// UI.h
#ifndef UI_H
#define UI_H

// librarys
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// arrays
static uint32_t scr[30] = {0};
static char font_data[2048] = {0};

static inline void tick(uint16_t tick_rate) {
#ifdef _WIN32
    Sleep(1000 / tick_rate);
#else
    usleep((uint64_t)(1000000 / tick_rate));
#endif
}

// terminal values
#ifndef _WIN32
static struct termios _old_tty;
static bool _tty_initialized = false;
static inline void _restore_tty(void) {
    if (_tty_initialized) {
        tcsetattr(STDIN_FILENO, TCSANOW, &_old_tty);
    }
}
#endif

// read keyboard
static inline char key(void) {
#ifdef _WIN32
    if (_kbhit()) {
        return (char)_getch();
    }
    return 0;
#else
    if (!_tty_initialized) {
        struct termios new_tty;
        tcgetattr(STDIN_FILENO, &_old_tty);
        new_tty = _old_tty;
        new_tty.c_lflag &= ~(ICANON | ECHO);
        new_tty.c_cc[VMIN] = 0;
        new_tty.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tty);
        _tty_initialized = true;
        atexit(_restore_tty);
    }
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) <= 0) {
        return 0; 
    }
    return ch;
#endif
}

static bool _mouse_initialized = false;
#ifndef _WIN32
static inline void _restore_mouse(void) {
    if (_mouse_initialized) {
        printf("\e[?1003l\e[?1000l");
        fflush(stdout);
    }
}
#endif

static inline bool mouse(uint8_t m[3]) {
#ifdef _WIN32
    (void)m;
    return false;
#else
    if (!_mouse_initialized) {
        printf("\e[?1000h\e[?1003h");
        fflush(stdout);
        _mouse_initialized = true;
        atexit(_restore_mouse);
        
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    char buf[6];
    ssize_t n = read(STDIN_FILENO, buf, 6);
    if (n >= 6) {
        if (buf[0] == 27 && buf[1] == '[' && buf[2] == 'M') {
            m[0] = (uint8_t)(buf[3] - 32);
            m[1] = (uint8_t)(buf[4] - 32);
            m[2] = (uint8_t)(buf[5] - 32);
            return true;
        }
    }
    return false;
#endif
}

// create pixel
static inline void pixel(uint8_t x, uint8_t y) {
    if (x < 30 && y < 30) {
        scr[y] |= (1U << x);
    } else {
        printf("pixel error\n");
    }
}

// create rect
static inline void rect(uint8_t r[4], bool full) {
    uint8_t x = r[0];
    uint8_t y = r[1];
    uint8_t w = r[2];
    uint8_t h = r[3];
    if (x + w <= 30 && y + h <= 30) {
        for (uint8_t Y = y; Y < y + h; Y++) {
            for (uint8_t X = x; X < x + w; X++) {
                if (full) {
                    pixel(X, Y);
                } else {
                    if (X == x || Y == y || X == x + w - 1 || Y == y + h - 1) {
                        pixel(X, Y);
                    }
                }
            }
        }
    } else {
        printf("rect error\n");
    }
}

// load font
static inline void font(char file[]) {
    FILE *f = fopen(file, "r"); 
    if (!f) {
        printf("font load error\n");
        return;
    }
    size_t bytes = fread(font_data, 1, sizeof(font_data) - 1, f);
    font_data[bytes] = '\0';
    fclose(f);
}

// output char
static inline void draw_char(char c, uint8_t start_x, uint8_t start_y) {
    if (font_data[0] == '\0') return;
    
    char search_pattern[16];
    snprintf(search_pattern, sizeof(search_pattern), "\"%c\": \"", c);
    
    char *start_pos = strstr(font_data, search_pattern);
    if (!start_pos) return; 
    
    start_pos += strlen(search_pattern);
    
    for (uint8_t row = 0; row < 5; row++) {
        unsigned int mask_val = 0;
        char *current_block = start_pos + (row * 6);
        
        if (current_block[0] == '\\' && current_block[1] == '\\') {
            current_block++;
        }
        
        if (sscanf(current_block, "\\u%04x", &mask_val) != 1) {
            if (sscanf(current_block, "u%04x", &mask_val) != 1) {
                continue;
            }
        }
        uint8_t mask = (uint8_t)mask_val;
        
        for (uint8_t col = 0; col < 3; col++) {
            if ((mask & (1 << (2 - col))) != 0) {
                pixel(start_x + col, start_y + row);
            }
        }
    }
}

// output text
static inline void text(char text_str[], uint8_t x, uint8_t y) {
    uint8_t current_x = x;
    size_t length = strlen(text_str);
    for (size_t i = 0; i < length; i++) {
        if (current_x + 3 > 30) break;
        if (y + 5 > 30) break;
        if (text_str[i] != ' ') {
            draw_char(text_str[i], current_x, y);
        }
        current_x += 4; 
    }
}

// collide rects
static inline bool colliderect(const uint8_t r1[4], const uint8_t r2[4]) {
    uint8_t x1 = r1[0], x2 = r2[0];
    uint8_t y1 = r1[1], y2 = r2[1];
    uint8_t w1 = r1[2], w2 = r2[2];
    uint8_t h1 = r1[3], h2 = r2[3];
    return ((int)x1 < (int)x2 + w2 &&
        (int)x1 + w1 > (int)x2 &&
        (int)y1 < (int)y2 + h2 &&
        (int)y1 + h1 > (int)y2);
}

//collide point
static inline bool collidepoint(const uint8_t r[4], uint8_t px, uint8_t py) {
    if (r == NULL) {return false;}
    uint8_t rx = r[0];
    uint8_t ry = r[1];
    uint8_t rw = r[2];
    uint8_t rh = r[3];
    return (px >= rx && px < rx + rw &&
            py >= ry && py < ry + rh);
}

// draw screen
static inline void draw(void) {
    for (uint8_t y = 0; y < 30; y++) {
        for (uint8_t x = 0; x < 30; x++) {
            if (((scr[y] >> x) & 1) == 1) {
                printf("##");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

// clear data screen
static inline void clear(void) {
    for (uint8_t id = 0; id < 30; id++) {
        scr[id] = 0;
    }
    printf("\033[2J\033[H");
}

#endif