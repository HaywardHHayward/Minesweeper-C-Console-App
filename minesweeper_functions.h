//
// Created by Hayden Reckward on 2/5/24.
//

#ifndef MINESWEEPER_MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_MINESWEEPER_FUNCTIONS_H

typedef unsigned char byte;

typedef struct tile {
    int row;
    int column;
    byte mineState;
    byte surroundingMines;
} tile_t;

typedef struct board {
    tile_t** array;
    int rowSize;
    int colSize;
    int numOfMines;
} board_t;


#endif //MINESWEEPER_MINESWEEPER_FUNCTIONS_H
