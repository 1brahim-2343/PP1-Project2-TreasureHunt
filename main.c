#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
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

int isInside(int row, int col) {
    if (row >= 0 && row < ROWS &&
        col >= 0 && col < COLS) {
        return 1;
    }
    return 0;
}

void placeObjects(int board[][COLS]) {
    int filledTreasuresCount = 0;
    while (filledTreasuresCount < TREASURES) {
        int row = rand() % ROWS, col = rand() % COLS;
        if (board[row][col] == EMPTY_BOARD) {
            board[row][col] = TREASURE_BOARD;
            filledTreasuresCount++;
        }
    }
    int filledTrapsCount = 0;
    while (filledTrapsCount < TRAPS) {
        int row = rand() % ROWS, col = rand() % COLS;
        if (board[row][col] == EMPTY_BOARD) {
            board[row][col] = TRAP_BOARD;
            filledTrapsCount++;
        }
    }
}

int countRegion(int board[][COLS], int row, int col, int target) {
    int targetCount = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int checkRow = i == 0 ? row : row + (int) pow(-1, i);
            int checkCol = j == 0 ? col : col + (int) pow(-1, j);

            if (isInside(checkRow, checkCol) &&
                board[checkRow][checkCol] == target) {
                targetCount++;
            }
        }
    }

    return targetCount;
}

int countNearbyTreasures(int board[][COLS], int row, int col) {
    return countRegion(board, row, col, TREASURE_BOARD);
}


int countTrapsInRegion(int board[][COLS], int row, int col) {
    return countRegion(board, row, col, TRAP_BOARD);
}

void printBoard(int board[][COLS], int revealed[][COLS], int showAll) {
    for (int i = 0; i < ROWS + 1; i++) {
        for (int j = 0; j < COLS + 2; j++) {
            if (i == 0 && j < 2) {
                printf("  ");
            } else if (i == 0) {
                printf(" %d", j - 2);
            } else if (j == 0) {
                printf("%d ", i - 1);
            } else if (j == 1) {
                printf("| ");
            } else {
                int row = i - 1;
                int col = j - 2;
                int isRevealed = revealed[row][col] == REVEALED_USER;

                printf(" ");
                if (board[row][col] == TREASURE_BOARD && (showAll || isRevealed)) {
                    printf("T");
                } else if (board[row][col] == TRAP_BOARD && (showAll || isRevealed)) {
                    printf("X");
                } else if (isRevealed) {
                    printf("%d", countNearbyTreasures(board, row, col));
                } else {
                    printf("#");
                }
            }
        }
        printf("\n");
    }
}

int revealCell(int board[][COLS], int revealed[][COLS], int row, int col) {
    revealed[row][col] = REVEALED_USER;
    return board[row][col];
}

void printStatus(int lives, int treasuresFound, int scannerUsed) {
    printf("Lives: %d Treasures: %d/%d Scanner: %s\n",
           lives, treasuresFound, TREASURES, scannerUsed ? "used" : "available");
}

char readAction(void) {
    char action;
    printf("Action (R = reveal, S = scanner): ");
    if (scanf(" %c", &action) != 1) {
        exit(0);
    }
    return action;
}

int readNumber(char *text) {
    int number;
    printf("%s", text);
    scanf("%d", &number);
    if (isdigit(number)) {
        printf("Input must be a number!\n");
        return -1;
    }

    return number;
}

void playGame(int board[][COLS], int revealed[][COLS]) {
    int lives = STARTING_LIVES;
    int treasuresFound = 0;
    int scannerUsed = 0;

    while (lives > 0 && treasuresFound < TREASURES) {
        printBoard(board, revealed, 0);
        printStatus(lives, treasuresFound, scannerUsed);
        char action = readAction();

        if (action == 'R' || action == 'r') {
            int row = readNumber("Row: ");
            int col = readNumber("Column: ");
            if (!isInside(row, col)) {
                printf("Coordinates must be from 0 to %d.\n", ROWS - 1);
                continue;
            }
            if (revealed[row][col] == REVEALED_USER) {
                printf("That cell was already explored. Choose another move.\n");
                continue;
            }
            int cell = revealCell(board, revealed, row, col);
            if (cell == TREASURE_BOARD) {
                printf("You found a treasure!\n");
                treasuresFound++;
            } else if (cell == TRAP_BOARD) {
                printf("Trap! You lost one life.\n");
                lives--;
            } else {
                printf("Empty cell. There are %d nearby treasures.\n",
                       countNearbyTreasures(board, row, col));
            }
        } else if (action == 'S' || action == 's') {
            if (scannerUsed) {
                printf("The scanner is unavailable. You already used it.\n");
                continue;
            }
            int row = readNumber("Scanner center row: ");
            int col = readNumber("Scanner center column: ");
            if (!isInside(row, col)) {
                printf("Coordinates must be from 0 to %d.\n", ROWS - 1);
                continue;
            }
            printf("Scanner result: %d treasures and %d traps in this region.\n",
                   countNearbyTreasures(board, row, col),
                   countTrapsInRegion(board, row, col));
            scannerUsed = 1;
        } else {
            printf("Invalid action. Use R or S.\n");
        }
    }

    if (treasuresFound == TREASURES) {
        printf("\nYou found all treasures. You win!\n");
    } else {
        printf("\nNo lives left. You lose!\n");
    }
    printBoard(board, revealed, 1);
}

int main(void) {
    srand(time(NULL));
    int board[ROWS][COLS];
    int revealed[ROWS][COLS];
    printf("TREASURE HUNT\n");
    initializeBoards(board, revealed);
    placeObjects(board);
    playGame(board, revealed);
    return 0;
}
