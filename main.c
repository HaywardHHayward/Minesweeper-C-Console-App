#include <stdio.h>
#include <stdlib.h>
#include "minesweeper_functions.h"

int main() {
    board_t* board = create_board(5, 5, 2);
    print_board(board);
}
