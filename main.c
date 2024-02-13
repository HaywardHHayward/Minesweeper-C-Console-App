#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "minesweeper_functions.h"

void check_tile_from_input(board_t* board, int* hit_mine) {
    int row, col;
    char input[MAX_BOARD_SIDE_LENGTH];
    char* end;
    int valid = 0;
    do {
        do {
            printf("Enter row: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            row = (int) strtol(input, &end, 10);
            if (row <= 0 || row > board->row_size + 1) {
                printf("Invalid input. The row selected must be an integer greater than zero and less than or equal to the amount of rows in the board. Try again.\n");
            }
            fflush(stdin);
        } while (row <= 0 || row > board->row_size + 1);
        do {
            printf("Enter column: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            col = (int) strtol(input, &end, 10);
            if (col <= 0 || col > board->col_size + 1) {
                printf("Invalid input. The column selected must be an integer greater than zero and less than or equal to the amount of columns in the board. Try again.\n");
            }
            fflush(stdin);
        } while (col <= 0 || col > board->col_size + 1);
        int result = check_tile(board, row - 1, col - 1);
        if (is_mine(get_tile(board, row - 1, col - 1))) {
            *hit_mine = 1;
        } else {
            *hit_mine = 0;
        }
        switch (result) {
            case CHANGE_CHECKED:
                printf("Cannot check a tile which has already been checked. Try again.\n");
                continue;
            case CHECK_FLAGGED:
                printf("Cannot check a tile which has been flagged. Unflag the tile if you wish to check it. Try again.\n");
                continue;
            case SUCCESS:
                valid = 1;
                break;
            default:
                printf("Invalid input.");
                continue;
        }
    } while (!valid);
}

void flag_tile_from_input(board_t* board) {
    fflush(stdin);
    int row, col;
    char input[MAX_BOARD_SIDE_LENGTH];
    char* end;
    int valid = 0;
    do {
        do {
            printf("Enter row: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            row = (int) strtol(input, &end, 10);
            if (row <= 0 || row > board->row_size + 1) {
                printf("Invalid input. The row selected must be an integer greater than or equal to zero and less than the amount of rows in the board. Try again.\n");
            }
            fflush(stdin);
        } while (row <= 0 || row > board->row_size + 1);
        do {
            printf("Enter column: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            col = (int) strtol(input, &end, 10);
            if (col <= 0 || col > board->col_size + 1) {
                printf("Invalid input. The column selected must be an integer greater than or equal to zero and less than the amount of columns in the board. Try again.\n");
            }
            fflush(stdin);
        } while (col <= 0 || col > board->col_size + 1);
        int result = flag_tile(board, row - 1, col - 1);
        switch (result) {
            case CHANGE_CHECKED:
                printf("Cannot flag a tile which has already been checked. Try again.\n");
                continue;
            case SUCCESS:
                valid = 1;
                break;
            default:
                printf("Invalid input.");
                continue;
        }
    } while (!valid);
}

int main(int argc, char** argv) {
    int row_size, col_size, mine_size;
    if (argc == 4) {
        char* end;
        row_size = (int) strtol(argv[1], &end, 10);
        if (!row_size) {
            perror("Invalid input");
            return -1;
        }
        col_size = (int) strtol(argv[2], &end, 10);
        if (!col_size) {
            perror("Invalid input");
            return -1;
        }
        mine_size = (int) strtol(argv[3], &end, 10);
        if (!mine_size) {
            perror("Invalid input");
            return -1;
        }
    } else if (argc == 1) {
        char input[MAX_BOARD_SIDE_LENGTH];
        char mine_input[MAX_MINE_INT_LENGTH];
        char* end;
        do {
            printf("Enter number of rows: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            row_size = (int) strtol(input, &end, 10);
            if (row_size <= 0) {
                printf("Invalid input. The number of rows must be an integer greater than zero. Try again.\n");
            }
            fflush(stdin);
        } while (row_size <= 0);
        do {
            printf("Enter number of columns: ");
            fgets(input, MAX_BOARD_SIDE_LENGTH, stdin);
            col_size = (int) strtol(input, &end, 10);
            if (col_size <= 0) {
                printf("Invalid input. The number of columns must be an integer greater than zero. Try again.\n");
            }
            fflush(stdin);
        } while (col_size <= 0);
        do {
            printf("Enter number of mines: ");
            fgets(mine_input, MAX_BOARD_SIDE_LENGTH, stdin);
            mine_size = (int) strtol(mine_input, &end, 10);
            if (mine_size <= 0 || mine_size >= row_size * col_size) {
                printf("Invalid input. The number of mines must be an integer in between zero and the number of rows multiplied by the number of columns. Try again.\n");
            }
            fflush(stdin);
        } while (mine_size <= 0 || mine_size >= row_size * col_size);
    } else {
        perror("Invalid number of arguments");
        return -1;
    }
    board_t* board = create_board(row_size, col_size, mine_size);
    int win = 0;
    while (1) {
        print_board(board);
        int valid = 0;
        int is_checking = 0;
        char input[MAX_COMMAND_LENGTH];
        while (!valid) {
            printf("Checking or flagging? (c/f, check/flag): ");
            fgets(input, MAX_COMMAND_LENGTH, stdin);
            fflush(stdin);
            for (int i = 0; input[i]; i++) {
                if (input[i] == '\n') {
                    input[i] = '\0';
                    break;
                }
                input[i] = (char) tolower(input[i]);
            }
            if (strcmp(input, "c") == 0 || strcmp(input, "check") == 0) {
                valid = 1;
                is_checking = 1;
            } else if (strcmp(input, "f") == 0 || strcmp(input, "flag") == 0) {
                valid = 1;
            } else {
                printf("Invalid input. Try again.\n");
            }
        }
        if (is_checking) {
            int hit_mine = 0;
            check_tile_from_input(board, &hit_mine);
            if (hit_mine) {
                break;
            }
        } else {
            flag_tile_from_input(board);
        }
        int flag_mines = flagged_all_mines(board);
        if (flag_mines) {
            win = 1;
            break;
        }
    }
    if (win) {
        printf("Congrats! You flagged all the mines! You win!\n");
    } else {
        printf("Oops! You hit a mine! You lost!\n");
    }
    free_board(board);
}
