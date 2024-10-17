
#include "minesweeper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Creates a new guess
 *
 * This function initializes a new guess data type with
 * specific horizontal and vertical (gHor and gVer) coordinates
 * which will be converted later
 *
 * @param gMode A char with the mode
 * @param gVer An unsigned short int for the guess vertical coordinate
 * @param gHor An unsigned short int for the guess horizontal coordinate
 * @param MAX_HOR
 * @return A pointer to the newly created guess
 */

Guess* createGuess(char gMode, unsigned short gVer, unsigned short gHor)
{
    // Validate input
    if (gMode != 'F' && gMode != 'D') {
        return NULL;
    } else if (gHor < 1 || gHor > MAX_HOR || gVer < 1 || gVer > MAX_VER) { // Check if coordinates are within bounds
        return NULL;
    }

    Guess* guess = (Guess*)malloc(sizeof(Guess));
    if (guess == NULL) {
        return NULL;
    }

    guess->hor = gHor - 1;
    guess->ver = gVer - 1;
    guess->mode = gMode;

    return guess;
}

/**
 * @brief frees the memory allocated to the guess
 *
 * This function deallocates the memory associated with the given guess,
 * which includes its horizontal and vertical coordinates, as well as
 * its mode
 *
 * @param g the guess which will be freed
 */

void freeGuess(Guess* g)
{
    free(g);
}

/**
 * @brief Creates a new game board with the specified dimensions.
 *
 * This function initializes a new game board with the given horizontal (hor)
 * and vertical (ver) dimensions. All cells in the board are initialized to 0.
 * Memory for the board structure and its data array is allocated dynamically.
 *
 * @param hor The horizontal dimension (number of columns) of the board.
 * @param ver The vertical dimension (number of rows) of the board.
 * @return A pointer to the newly created Board structure, or NULL if memory allocation fails.
 */

Board* createBoard(unsigned short hor, unsigned short ver)
{
    // board is too small here, needs to be at least 2 by 2
    if (hor < 2 || ver < 2) {
        return NULL;
    }
    Board* board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        return NULL;
    }

    board->hor = hor;
    board->ver = ver;
    board->data = (unsigned short**)malloc(hor * sizeof(unsigned short*));
    if (board->data == NULL) {
        free(board);
        return NULL;
    }

    for (size_t i = 0; i < hor; i++) {
        board->data[i] = (unsigned short*)malloc(ver * sizeof(unsigned short));
        if (board->data[i] == NULL) {
            for (size_t j = 0; i > 0 && j < i; j++) {
                free(board->data[j]);
            }
            free(board->data);
            free(board);
            return NULL;
        }
    }

    for (size_t i = 0; i < hor; i++) {
        for (size_t j = 0; j < ver; j++) {
            board->data[i][j] = 0;
        }
    }

    return board;
}

/**
 * @brief Frees the memory allocated for the game board.
 *
 * This function deallocates the memory associated with the given Board structure,
 * including the memory allocated for its data array. It ensures that all allocated
 * memory is properly freed to avoid memory leaks.
 *
 * @param board A pointer to the Board structure to be freed. If the pointer is NULL,
 *              the function does nothing.
 */

void freeBoard(Board* board)
{
    if (board != NULL) {
        for (size_t i = 0; i < board->hor; i++) {
            free(board->data[i]);
        }
        free(board->data);
        free(board);
    }
}

/**
 * @brief Creates a game board with the specified dimensions and randomly places bombs.
 *
 * This function initializes a new game board with the given horizontal (hor) and vertical (ver)
 * dimensions and randomly places the specified number of bombs (bomb_count) on the board.
 * Bombs are represented by the value 9. The central cell is ensured to be bomb-free.
 *
 * @param hor The horizontal dimension (number of columns) of the board.
 * @param ver The vertical dimension (number of rows) of the board.
 * @param bomb_count The number of bombs to be placed on the board.
 * @return A pointer to the newly created Board structure, or NULL if memory allocation fails.
 */

Board* createBombBoard(unsigned short hor, unsigned short ver, unsigned int bomb_count)
{
    if (bomb_count >= ver * hor) {
        return NULL;
    }
    Board* board = createBoard(hor, ver);

    srand((unsigned int)time(NULL));
    unsigned short mid_h = (hor / 2) - 1;
    unsigned short mid_v = (ver / 2) - 1;
    for (int i = 0; i < bomb_count; i++) {
        int rand_x = rand() % hor;
        int rand_y = rand() % ver;
        while (board->data[rand_x][rand_y] == 9 || (rand_x == mid_h && rand_y == mid_v)) {
            rand_x = rand() % hor;
            rand_y = rand() % ver;
        }
        board->data[rand_x][rand_y] = 9;
    }

    return board;
}

/**
 * @brief Populates the board with the number of bombs in adjacent cells.
 *
 * This function calculates the number of bombs adjacent to each cell on the board
 * and updates each cell with this count. Cells containing bombs (value 9) are not updated.
 *
 * @param board A pointer to the Board structure whose cells are to be populated.
 */

void populateNumberReferences(Board* board)
{
    int directions[8][2] = {
        { -1, -1 }, { -1, 0 }, { -1, 1 },
        { 0, -1 }, { 0, 1 },
        { 1, -1 }, { 1, 0 }, { 1, 1 }
    };

    for (int i = 0; i < board->hor; i++) {
        for (int j = 0; j < board->ver; j++) {
            if (board->data[i][j] == 9) {
                continue;
            }

            int bombCount = 0;

            for (int d = 0; d < 8; d++) {
                int newRow = i + directions[d][0];
                int newCol = j + directions[d][1];

                if (newRow >= 0 && newRow < board->hor && newCol >= 0 && newCol < board->ver) {
                    if (board->data[newRow][newCol] == 9) {
                        bombCount++;
                    }
                }
            }
            board->data[i][j] = bombCount;
        }
    }
}

/**
 * @brief Prints the game board to the console.
 *
 * This function prints the game board to the console, with bombs represented by an asterisk (*)
 * and other cells showing the number of adjacent bombs.
 *
 * @param board A pointer to the Board structure to be printed.
 */

void printBoard(Board* board)
{
    for (int j = board->ver - 1; j >= 0; j--) {
        for (int i = 0; i < board->hor; i++) {
            if (board->data[i][j] == 9) {
                printf(" *");
            } else {
                printf(" %hu", board->data[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Prints the game board to the console for the user.
 *
 * This function prints the game board to the console, with flags represented by an 'F'
 * and other cells showing X as undiscovered
 *
 * @param board A pointer to the Board structure of the bombs.
 * @param board A pointer to the Board structure of the flags.
 * @param board A pointer to the Board structure of the revealed squares.
 */

void printTotalBoard(Board* bomb, Board* flag, Board* usermask)
{
    for (int j = bomb->ver - 1; j >= 0; j--) {
        for (int i = 0; i < bomb->hor; i++) {
            if (usermask->data[i][j] == 1) {
                if (flag->data[i][j] == 1) {
                    printf(" F");
                } else {
                    printf(" %hu", bomb->data[i][j]);
                }
            } else {
                printf(" X");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Destroys spaces around a box if it is empty
 *
 * This function will propogate non-bomb spaces and destroy them
 * if, stopping when it is a bomb
 * @param bombB A pointer to the Board structure containing the bomb placements.
 * @param flagB A pointer to the Board structure containing the flag placements.
 * @param board A pointer to the Board structure of the revealed squares.
 * @param hor An unsigned int for the horizontal coordinate of the square.
 * @param ver An unsigned int for the vertical coordinate of the square.
 */

void destroySpacesAround(Board* bombB, Board* flagB, Board* maskB,
    unsigned short hor, unsigned short ver, bool first_guess)
{
    int directions[8][2] = {
        { -1, -1 }, { -1, 0 }, { -1, 1 },
        { 0, -1 }, { 0, 1 },
        { 1, -1 }, { 1, 0 }, { 1, 1 }
    };

    // Check bounds first
    // redundant?
    if (hor < 0 || hor >= bombB->hor || ver < 0 || ver >= bombB->ver) {
        return;
    }

    // If the space is already visible or flagged, do nothing
    if (maskB->data[hor][ver] == 1 || flagB->data[hor][ver] == 1) {
        return;
    }

    // Mark the current cell as visible
    maskB->data[hor][ver] = 1;

    // If the space is not a bomb and is empty, recursively check surrounding cells
    if (bombB->data[hor][ver] == 0) {
        for (int dir = 0; dir < 8; dir++) {
            unsigned short newHor = hor + directions[dir][0];
            unsigned short newVer = ver + directions[dir][1];
            destroySpacesAround(bombB, flagB, maskB, (hor + directions[dir][0]), (ver + directions[dir][1]), false);
        }
    }
}

/**
 * @brief Processes a user's guess and determines if it hits a bomb.
 *
 * This function handles the user's guess by checking for validity,
 * and determining if the guess hits a bomb. It prints relevant information for debugging and
 * returns whether the guess was a bomb.
 *
 * @param bombB A pointer to the Board structure containing the bomb placements.
 * @param flagB A pointer to the Board structure containing the flag placements.
 * @param board A pointer to the Board structure of the revealed squares.
 * @return true if the guess hits a bomb, false otherwise.
 */

bool takeTurn(Board* bombB, Board* flagB, Board* maskB)
{
    printf("Enter 'F' or 'D' (Flag or Destroy), followed by coordinates, starting at 1,1 from the bottom left.\n");
    printf("Ex: F 1,1 -> flags the bottom left hand corner\n");

    unsigned short gVer;
    unsigned short gHor;
    char gMode;
    // scanf_s needs a character size buffer, that is what the 1 is for
    int result = scanf_s(" %c %hu,%hu", &gMode, 1, &gHor, &gVer);
    Guess* guess;
    if (result == 3) {
        guess = createGuess(gMode, gVer, gHor);
    } else {
        return false;
    }
    if (guess == NULL) {
        return false;
    }

    unsigned short guessHor = guess->hor;
    unsigned short guessVer = guess->ver;

    // Additional check to ensure coordinates are valid
    if (guessHor >= bombB->hor || guessVer >= bombB->ver) {
        freeGuess(guess);
        return false;
    }
    bool retVal;
    if (guess->mode == 'F') { // flag mode
        maskB->data[guessHor][guessVer] = 1;
        flagB->data[guessHor][guessVer] = 1;
        retVal = false;
    } else { // destroy mode
        maskB->data[guessHor][guessVer] = 0;
        flagB->data[guessHor][guessVer] = 0;
        retVal = (bombB->data[guessHor][guessVer] == 9);
        destroySpacesAround(bombB, flagB, maskB, guessHor, guessVer, true);
    }
    freeGuess(guess);
    return retVal;
}

/**
 * @brief Compares the bombs and flags
 *
 * This function checks to see if the flags and bombs match
 *
 * @param bombCount A pointer to an uint for the bomb count
 * @param bomb A pointer to the Board structure containing bombs.
 * @param flag A pointer to the Board structure containing flags.
 */

bool compBoards(unsigned int bombCount, Board* bomb, Board* flag)
{
    unsigned int flagCount = 0;
    for (int x = 0; x < bomb->hor; x++) {
        for (int y = 0; y < bomb->ver; y++) {
            if (bomb->data[x][y] == 9 && flag->data[x][y] != 1) {
                return false;
            } else if (bomb->data[x][y] == 9) {
                flagCount++;
            }
        }
    }
    return flagCount == bombCount;
}
