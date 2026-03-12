#ifndef GAME_H
#define GAME_H

#include <M5StickC.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "EEPROM.h"
#include "board.h"
#include <stdbool.h>

#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 50
#define MIN_TILT 0.3
#define SPEED 3
#define EEPROM_GAME_STATE_ADDRESS 0

struct EEPROMGameState
{
    game_state gameState;
    Cursor cursor;
};

void move_position(float acc_x, float acc_y);
int find_color(int x, int y);
void drawBoardOnStick();
void draw_text();
void updateCursorPosition();
void drawCursor();
void rotate_cursor();
void swap_color();
void update_score();
void update_moves();
void draw_level();
void begin_level();
void new_level();
void game_over();
void restart();
void look_mode();
void draw_menu();
void highlightInstruction(int currentInstruction, int color);
void wait_input_menu(int curr);
void save_game();
void load_game();
#endif // GAME_H
