#ifndef MINESWEEPER_MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_MINESWEEPER_FUNCTIONS_H

#include "haywardstd.h"

typedef unsigned char byte;

#define IS_CHECKED (1 << 0)
#define IS_FLAGGED (1 << 1)
#define IS_MINE (1 << 2)
#define SURROUNDING_MINES_SHIFT 4
#define SURROUNDING_MINES_BITMASK 0b1111
#define SURROUNDING_MINES (SURROUNDING_MINES_BITMASK << SURROUNDING_MINES_SHIFT)

typedef struct tile {
    int row;
    int column;
    byte mine_state;
} tile_t;

typedef struct board {
    tile_t** array;
    int row_size;
    int col_size;
    int mine_num;
    array_t* mine_tiles;
    array_t* flag_tiles;
} board_t;

int compare_tiles(const void* t1, const void* t2);

int is_checked(tile_t* tile);

int is_flagged(tile_t* tile);

int is_mine(tile_t* tile);

void set_surrounding_mines(tile_t* tile, byte mine_count);

int get_surrounding_mines(tile_t* tile);

int toggle_checked(tile_t* tile);

int toggle_flagged(tile_t* tile);

int toggle_mine(tile_t* tile);

board_t* create_board(int row, int col, int mines);

tile_t* get_tile(board_t* board, int row, int col);

void print_board(board_t* board);

#endif //MINESWEEPER_MINESWEEPER_FUNCTIONS_H
