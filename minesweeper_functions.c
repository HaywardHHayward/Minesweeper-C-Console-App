#include <stdlib.h>
#include "minesweeper_functions.h"

int is_checked(tile_t* tile) {
    return tile->mine_state & IS_CHECKED;
}

int is_flagged(tile_t* tile) {
    return tile->mine_state & IS_FLAGGED;
}

int is_mine(tile_t* tile) {
    return tile->mine_state & IS_MINE;
}

static inline void toggle_checked(tile_t* tile) {
    tile->mine_state ^= IS_CHECKED;
}

static inline void toggle_flagged(tile_t* tile) {
    tile->mine_state ^= IS_FLAGGED;
}

static inline void toggle_mine(tile_t* tile) {
    tile->mine_state ^= IS_MINE;
}

void set_surrounding_mines(tile_t* tile, byte mine_count) {
    tile->mine_state &= ((0b11111111) & ~(SURROUNDING_MINES_BITMASK << SURROUNDING_MINES_SHIFT));
    tile->mine_state |= ((mine_count & SURROUNDING_MINES_BITMASK) << SURROUNDING_MINES_SHIFT);
}

int get_surrounding_mines(tile_t* tile) {
    return (tile->mine_state & SURROUNDING_MINES) >> SURROUNDING_MINES_SHIFT;
}

tile_t* get_tile(board_t* board, int row, int col) {
    if (row < 0 || row >= board->row_size) {
        return NULL;
    }
    if (col < 0 || col >= board->col_size) {
        return NULL;
    }
    return &board->array[row][col];
}

int flag_tile(board_t* board, int row, int col) {
    tile_t* tile = get_tile(board, row, col);
    if (!tile) {
        return INVALID_TILE;
    }
    if (is_checked(tile)) {
        return CHANGE_CHECKED;
    }
    toggle_flagged(tile);
    return SUCCESS;
}

int check_tile(board_t* board, int row, int col) {
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
    return SUCCESS;
}

static inline void add_surrounding_mines(board_t* board, tile_t* mine) {
    int row = mine->row;
    int col = mine->column;
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

board_t* create_board(int row, int col, int mines) {
    if (row <= 0 || col <= 0 || mines > row * col) {
        perror("Invalid input");
        abort();
    }
    srand(time(0));
    board_t* board = (board_t*) calloc(1, sizeof(board_t));
    board->array = (tile_t**) calloc(row, sizeof(tile_t*));
    board->row_size = row;
    board->col_size = col;
    board->mine_tiles = create_array_with_length(sizeof(tile_t), mines);
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
        int rand_row = random_number(0, row);
        int rand_col = random_number(0, col);
        tile_t* random_tile = get_tile(board, rand_row, rand_col);
        if (contained_in_array(board->mine_tiles, (void*) random_tile, compare_tiles)) {
            continue;
        }
        toggle_mine(random_tile);
        add_to_array(board->mine_tiles, (void*) random_tile);
    }
    for (int i = 0; i < board->mine_tiles->size; i++) {
        add_surrounding_mines(board, (tile_t*) get_array_index(board->mine_tiles, i));
    }
    return board;
}

int compare_tiles(const void* t1, const void* t2) {
    tile_t* tile1 = (tile_t*) t1;
    tile_t* tile2 = (tile_t*) t2;
    int same_position = (tile1->row == tile2->row) && (tile1->column == tile2->column);
    if (!same_position) {
        return -1;
    }
    int same_state = tile1->mine_state == tile2->mine_state;
    return same_state ? 0 : 1;
}

char tile_repr(tile_t* tile) {
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
    return (char) (get_surrounding_mines(tile) + INT_TO_CHAR_OFFSET);
}

void print_board(board_t* board) {
    for (int i = 0; i < board->row_size; i++) {
        for (int j = 0; j < board->col_size; j++) {
            printf("%c ", tile_repr(get_tile(board, i, j)));
        }
        printf("\n");
    }
}