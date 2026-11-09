#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 8
#define COLS 8
#define TREASURES 5
#define TRAPS 3
#define STARTING_LIVES 3

#define EMPTY_BOARD 0
#define TREASURE_BOARD 1
#define TRAP_BOARD (-1)
#define HIDDEN_USER 0
#define REVEALED_USER 1


void initializeBoards(int board[][COLS], int revealed[][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = EMPTY_BOARD;
            revealed[i][j] = HIDDEN_USER;
        }
    }
}

int isInside(int col, int row) {
    if (col >= 0 && col < COLS &&
        row >= 0 && row < ROWS) {
        return 1;
    }
    return 0;
}

void placeObjects(int board[][COLS]) {
    int filledTreasuresCount = 0;
    while (filledTreasuresCount < TREASURES) {
        const int row = rand() % ROWS, col = rand() % COLS;
        if (board[row][col] == EMPTY_BOARD) {
            board[row][col] = TREASURE_BOARD;
            filledTreasuresCount++;
        }
    }
    int filledTrapsCount = 0;
    while (filledTrapsCount < TRAPS) {
        const int row = rand() % ROWS, col = rand() % COLS;
        if (board[row][col] == EMPTY_BOARD) {
            board[row][col] = TRAP_BOARD;
            filledTrapsCount++;
        }
    }
}

int countNearbyTreasures(int board[][COLS], int row, int col) {
    int nearbyTreasuresCount = 0;
    //non-edge, non-corner
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 0 && j == 0) continue;
            const int checkRow = i == 0 ? row : row + (int) pow(-1, i);
            const int checkCol = j == 0 ? col : col + (int) pow(-1, j);
            if (board[checkRow][checkCol] == TREASURE_BOARD) {
                nearbyTreasuresCount++;
            }
        }
    }
    //edge

    //corner
}

void printBoard(int board[][COLS], int revealed[][COLS], int showAll) {
    if (showAll) {
        printf("Correct board\n");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                if (i == 0 && j < 2) {
                    printf("  ");
                } else if (i == 0) {
                    printf(" %d", j - 2);
                } else if (j == 0) {
                    printf("%d ", i - 1);
                } else if (j == 1) {
                    printf("| ");
                } else {
                    printf(" ");

                    printf("%c", board[i - 1][j - 2] == TREASURE_BOARD
                                     ? 'T'
                                     : board[i - 1][j - 2] == TRAP_BOARD
                                           ? 'X'
                                           : '#');
                }
            }
            printf("\n");
        }
    }
    // showALL not completed, print surrounding treasure count for revealed cells
}


int main(void) {
    srand(time((NULL)));
    int board[ROWS][COLS];
    int revealed[ROWS][COLS];
    initializeBoards(board, revealed);
    placeObjects(board);
    printBoard(board, revealed, 1);
    return 0;
}
