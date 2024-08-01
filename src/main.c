#include <stdbool.h>
#include <stdio.h>
#include "minesweeper.h"

int main()
{
    printf("Welcome to Minesweeper.\n");
    printf("Please type the dimension of the board as two comma-separated values (EX: 10,5):\n");

    unsigned short hor, ver;
    if (scanf_s("%hu,%hu", &hor, &ver) != 2) {
        printf("Invalid input. Please enter two integer values separated by a comma.\n");
        return 1;
    }

    if (hor <= 2 || ver <= 2 || hor > 65535 || ver > 65535) {
        printf("Dimensions must be positive integers between 2 and 65535.\n");
        return 1;
    }

    unsigned int bomb_count;
    printf("Please type the amount of bombs from 1 to %d.\n", (hor * ver - 1));
    if (scanf_s("%d", &bomb_count) != 1 || bomb_count < 1 || bomb_count > (hor * ver - 1)) {
        printf("Invalid bomb count. It must be a positive integer between 1 and %d.\n", (hor * ver - 1));
        return 1;
    }

    Board* bombBoard = createBombBoard(hor, ver, bomb_count);
    Board* maskBoard = createBoard(hor, ver);
    Board* flagBoard = createBoard(hor, ver);

    populateNumberReferences(bombBoard);

    // DEBUG LINE
    printf("DEBUG:\n");
    printBoard(bombBoard);
    printf("\n");

    bool gameOver = false;
    char* message = "";
    while (!gameOver) {
        printTotalBoard(bombBoard, flagBoard, maskBoard);

        if (takeTurn(bombBoard, flagBoard, maskBoard)) {
            gameOver = true;
            message = "\nYou hit a bomb! Game Over!\n";
        } else if (compBoards(bomb_count, bombBoard, flagBoard)) {
            gameOver = true;
            message = "\nYou identified all the bombs! You win!\n";
        }
    }

    printf_s(message);
    printf("\n\n");

    // shows the complete board when done
    for (int i = 0; i < maskBoard->hor; i++) {
        for (int j = 0; j < maskBoard->ver; j++) {
            if (maskBoard->data[i][j] == 0) {
                maskBoard->data[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < flagBoard->hor; i++) {
        for (int j = 0; j < flagBoard->ver; j++) {
            if (bombBoard->data[i][j] == 9) {
                flagBoard->data[i][j] = 1;
            } else {
                flagBoard->data[i][j] = 0;
            }
        }
    }
    printTotalBoard(bombBoard, flagBoard, maskBoard);

    freeBoard(bombBoard);
    freeBoard(maskBoard);
    freeBoard(flagBoard);

    return 0;
}
