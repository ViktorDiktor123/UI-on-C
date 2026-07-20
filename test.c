#include "UI.h"
#include <string.h>

uint8_t ramaUp[4] = {0, 5, 30, 1};
uint8_t ramaDown[4] = {0, 29, 30, 1};

bool mario1[5][5] = {
    {0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1}
};
bool mario2[5][5] = {
    {0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0}
};
bool mario3[5][5] = {
    {1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 1, 0, 0}
};
bool mario[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

bool img [5][15] = {
    {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0},
};

uint8_t time = 0;
bool smena = 1;
uint8_t kadr = 1;
uint8_t player[4] = {0, 24, 5, 5};
uint8_t win[4] = {25, 9, 5, 5};
uint8_t ship[3][4] = {
    {20, 19, 22, 14},
    {22, 14, 24, 19},
    {24, 19, 20, 19},
};
bool w = false;
bool jmp = 0;
uint8_t jump_timer;
uint8_t foot[4] = {10, 19, 20, 11};

uint8_t r[4][4] = {
    {5, 24, 5, 5},
    {10, 19, 5, 5},
    {15, 14, 5, 5},
    {25, 14, 5, 5},
};

void marioDraw() {
    if (time == 5) {
        if (smena) {
            kadr++;
        } else {
            kadr--;
        }

        if (kadr == 2) {
            memcpy(mario, mario3, sizeof(mario));
            smena = !smena;
        } else if (kadr == 0) {
            memcpy(mario, mario1, sizeof(mario));
            smena = !smena;
        } else {
            memcpy(mario, mario2, sizeof(mario));
        }

        time = 0;
    }

    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (mario[x][y]) {
                pixel(player[0] + y, player[1] + x);
            }
        }
    }
    
    time++;
}

uint8_t soon[3] = {3, 9, 3};

int main() {
    font("font.json");
    
    while (1) {
        clear();
        char ch = key();
        jmp = !jmp;
        
        circle(soon, true);
        rect(ramaUp, true);
        rect(ramaDown, true);
        for (uint8_t a = 0; a < 4; a++) {
            rect(r[a], true);
        }
        rect(win, false);
        
        if (colliderect(player, win)) {
            memcpy(player, win, sizeof(win));
            rect(win, true);
            text("YOU WIN", 0, 0);
            w = true;
            for (uint8_t y = 0; y < 5; y++) {
                for (uint8_t x = 0; x < 15; x++) {
                    if (img[y][x]) {
                        pixel(x+15, y+22);
                    }
                }
            }
        } else {
            text("MARIO", 0, 0);
        }
        
        if (ch == 'w') {
            jump_timer = 8;
        } else if (ch == 'a') {
            player[0]--;
            for (uint8_t a = 0; a < 5; a++) {
                if (colliderect(player, r[a])) {
                    player[0]++;
                }
                if (player[0] == 255) {player[0] = 0;}
            }
        } else if (ch == 'd') {
            player[0]++;
            for (uint8_t a = 0; a < 5; a++) {
                if (colliderect(player, r[a])) {
                    player[0]--;
                }
                if (player[0] > 30) {player[0] = 30;}
            }
        }
        
        if (jump_timer > 0) {
            player[1]--;
            jump_timer--;
        } else {
            if (jmp) { player[1]++; }
        }
        
        for (uint8_t a = 0; a < 5; a++) {
            if (colliderect(player, r[a])) {
                player[1]--;
            }
        }
        
        for (uint8_t a = 0; a < 3; a++) {
            line(ship[a]);
            if (collideline(player, ship[a])) {
                memcpy(player, (uint8_t[]){0, 24, 5, 5}, sizeof(player));
            }
        }
        
        if (colliderect(foot, player)) {
            player[1] -= 2;
        }
        
        if (player[0] > 25) {
            player[0] = 25;
        } else if (player[0] == 255) {
            player[0] = 0;
        }
        
        if (player[1] > 24) {
            player[1] = 24;
        } else if (player[1] == 255) {
            player[1] = 0;
        }
        
        marioDraw();
        
        tick(60);
        draw();
        if (w) {break;}
    }
    
    return 0;
}
