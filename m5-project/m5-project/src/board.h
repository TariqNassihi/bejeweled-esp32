// game_board.h

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int x;
    int y;
    bool rotate;
} Cursor;

typedef struct
{
    int cell_size;
    int moves_left_init;
    int moves_left;
    int score;
    int level;
    bool always_swap;
    int size_row;
    int size_column;
    int number_color;
    bool should_continue;
    int score_needed;
} game_state;

extern game_state game;
extern Cursor cursor;

extern int **board;

void place_diamonds(void);
void check_pattern_board(void);

void change_color(int x, int y, int color);
void initialize_board(void);
void print_board(void);
void check_three_in_a_row(int x, int y);
void check_board();
void check_pattern_after_swap(int position1_x, int position1_y, int position2_x, int position2_y);
void allocate_board();
void free_board();
void swap_numbers(int position1_x, int position1_y, int position2_x, int position2_y);
void remove_and_replace_patterns();
void check_special_diamond(int x, int y);
void delete_horizontal(int y);
void delete_vertical(int x);
void shift_down();

#endif // GAME_BOARD_H
