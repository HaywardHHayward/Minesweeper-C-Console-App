#include "minesweeper_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

int is_checked(const tile_t* tile) {
    return tile->mine_state & IS_CHECKED;
}

int is_flagged(const tile_t* tile) {
    return tile->mine_state & IS_FLAGGED;
}

int is_mine(const tile_t* tile) {
    return tile->mine_state & IS_MINE;
}

static void toggle_checked(tile_t* tile) {
    tile->mine_state ^= IS_CHECKED;
}

static void toggle_flagged(tile_t* tile) {
    tile->mine_state ^= IS_FLAGGED;
}

static void toggle_mine(tile_t* tile) {
    tile->mine_state ^= IS_MINE;
}

void set_surrounding_mines(tile_t* tile, const byte mine_count) {
    tile->mine_state &= 0b11111111 & ~(SURROUNDING_MINES_BITMASK << SURROUNDING_MINES_SHIFT);
    tile->mine_state |= (mine_count & SURROUNDING_MINES_BITMASK) << SURROUNDING_MINES_SHIFT;
}

int get_surrounding_mines(const tile_t* tile) {
    return (tile->mine_state & SURROUNDING_MINES) >> SURROUNDING_MINES_SHIFT;
}

tile_t* get_tile(const board_t* board, const int row, const int col) {
    if (row < 0 || row >= board->row_size) {
        return NULL;
    }
    if (col < 0 || col >= board->col_size) {
        return NULL;
    }
    return &board->array[row][col];
}

int flag_tile(const board_t* board, const int row, const int col) {
    tile_t* tile = get_tile(board, row, col);
    if (!tile) {
        return INVALID_TILE;
    }
    if (is_checked(tile)) {
        return CHANGE_CHECKED;
    }
    toggle_flagged(tile);
    if (remove_from_array(board->flag_tiles, tile, compare_tiles) == 0) {
        add_to_array(board->flag_tiles, tile);
    }
    return SUCCESS;
}

static void check_surrounding_tiles(board_t* board, const int row, const int col) {
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0) {
                continue;
            }
            check_tile(board, row + r, col + c);
        }
    }
}

int check_tile(board_t* board, const int row, const int col) {
    tile_t* tile = get_tile(board, row, col);
    if (!tile) {
        return INVALID_TILE;
    }
    if (is_checked(tile)) {
        return CHANGE_CHECKED;
    }
    if (is_flagged(tile)) {
        return CHECK_FLAGGED;
    }
    toggle_checked(tile);
    if (!is_mine(tile) && get_surrounding_mines(tile) == 0) {
        check_surrounding_tiles(board, row, col);
    }
    return SUCCESS;
}


static void add_surrounding_mines(const board_t* board, const tile_t* mine) {
    const int row = mine->row;
    const int col = mine->column;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            tile_t* tile = get_tile(board, row + i, col + j);
            if (!tile) {
                continue;
            }
            if (is_mine(tile)) {
                continue;
            }
            set_surrounding_mines(tile, get_surrounding_mines(tile) + 1);
        }
    }
}

board_t* create_board(const int row, const int col, const int mines) {
    if (row <= 0 || col <= 0 || mines > row * col) {
        perror("Invalid input");
        abort();
    }
    srand(time(0));
    board_t* board = calloc(1, sizeof(board_t));
    board->array = (tile_t**) calloc(row, sizeof(tile_t*));
    board->row_size = row;
    board->col_size = col;
    board->mine_tiles = create_array(sizeof(tile_t));
    board->flag_tiles = create_array(sizeof(tile_t));
    for (int r = 0; r < row; r++) {
        board->array[r] = (tile_t*) calloc(col, sizeof(tile_t));
        for (int c = 0; c < col; c++) {
            tile_t* tile = get_tile(board, r, c);
            tile->row = r;
            tile->column = c;
        }
    }
    while (board->mine_tiles->size < mines) {
        const int rand_row = random_number(0, row);
        const int rand_col = random_number(0, col);
        tile_t* random_tile = get_tile(board, rand_row, rand_col);
        if (contained_in_array(board->mine_tiles, random_tile, compare_tiles)) {
            continue;
        }
        toggle_mine(random_tile);
        add_to_array(board->mine_tiles, random_tile);
    }
    for (int i = 0; i < board->mine_tiles->size; i++) {
        add_surrounding_mines(board, get_array_index(board->mine_tiles, i));
    }
    return board;
}

int compare_tiles(const void* t1, const void* t2) {
    const tile_t* tile1 = (tile_t*) t1;
    const tile_t* tile2 = (tile_t*) t2;
    return !(tile1->row == tile2->row && tile1->column == tile2->column);
}

char tile_repr(const tile_t* tile) {
    if (is_flagged(tile)) {
        return 'P';
    }
    if (!is_checked(tile)) {
        return 'O';
    }
    if (is_mine(tile)) {
        return 'M';
    }
    if (get_surrounding_mines(tile) == 0) {
        return ' ';
    }
    return get_surrounding_mines(tile) + INT_TO_CHAR_OFFSET;
}

void print_board(const board_t* board) {
    char row_str[MAX_BOARD_SIDE_LENGTH];
    char col_str[MAX_BOARD_SIDE_LENGTH];
    sprintf(row_str, "%d", board->row_size);
    sprintf(col_str, "%d", board->col_size);
    unsigned int row_length = strlen(row_str);
    unsigned int col_length = strlen(col_str);
    printf("%*c", row_length, ' ');
    for (int i = 0; i < board->col_size; i++) {
        printf("%*d", col_length + 1, i + 1);
    }
    printf("\n");
    for (int i = 0; i < board->row_size; i++) {
        printf("%-*d", row_length, i + 1);
        for (int j = 0; j < board->col_size; j++) {
            printf("%*c", col_length + 1, tile_repr(get_tile(board, i, j)));
        }
        printf("\n");
    }
}

void free_board(board_t* board) {
    free_array(board->mine_tiles);
    free_array(board->flag_tiles);
    for (int r = 0; r < board->row_size; r++) {
        free(board->array[r]);
    }
    free(board->array);
    free(board);
}

int flagged_all_mines(const board_t* board) {
    return array_set_equal(board->mine_tiles, board->flag_tiles, compare_tiles);
}
