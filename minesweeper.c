
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    unsigned short hor;
    unsigned short ver;
    unsigned short** data;
} Board;

typedef struct {
    unsigned short hor;
    unsigned short ver;
    char mode;
} Guess;

/**
 * @brief Creates a new guess
 *
 * This function initializes a new guess data type with
 * specific horizontal and vertical (gHor and gVer) coordinates
 * which will be converted later
 *
 * @return A pointer to the newly created guess
 */

Guess* createGuess(unsigned short maxHor, unsigned short maxVer)
{
    printf("Enter 'F' or 'D' (Flag or Destroy), followed by coordinates, starting at 1,1 from the bottom left.\n");
    printf("Ex: F 1,1 -> flags the bottom left hand corner\n");

    unsigned short gVer;
    unsigned short gHor;
    char gMode;
    // scanf_s needs a character size buffer, that is what the 1 is for
    int result = scanf_s(" %c %hu,%hu", &gMode, 1, &gHor, &gVer);
    printf("User Guess: Mode: %c, Hor: %hu, Ver: %hu\n", gMode, gHor, gVer);

    // Validate input
    if (result != 3 || (gMode != 'F' && gMode != 'D')) {
        printf("Invalid input. Please enter a valid mode and coordinates.\n");
        //return NULL;
    }

    // Check if coordinates are within bounds
    else if (gHor < 1 || gHor > maxHor || gVer < 1 || gVer > maxVer) {
        printf("Coordinates out of bounds. Please enter coordinates within the board dimensions.\n");
        //return NULL;
    }

    Guess* guess = (Guess*)malloc(sizeof(Guess));
    if (guess == NULL) {
        fprintf(stderr, "Failed to allocate memory for guess structure\n");
        //return NULL;
    }

    guess->hor = gHor - 1;
    guess->ver = gVer - 1;
    guess->mode = gMode;

    // Debugging output
    printf("User Guess: Mode: %c, Hor: %hu, Ver: %hu\n", gMode, gHor, gVer);
    printf("Converted Guess: Hor: %hu, Ver: %hu\n", guess->hor, guess->ver);

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
    Board* board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        fprintf(stderr, "Failed to allocate memory for board structure\n");
        return NULL;
    }

    board->hor = hor;
    board->ver = ver;
    board->data = (unsigned short**)malloc(hor * sizeof(unsigned short*));
    if (board->data == NULL) {
        fprintf(stderr, "Failed to allocate memory for board data pointers\n");
        free(board);
        return NULL;
    }

    for (size_t i = 0; i < hor; i++) {
        board->data[i] = (unsigned short*)malloc(ver * sizeof(unsigned short));
        if (board->data[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for board row %zu\n", i);
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

Board* createBinaryBoard(unsigned short hor, unsigned short ver, unsigned int bomb_count)
{
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
 * @brief Converts user guess coordinates to board coordinates.
 *
 * This function converts the user-provided guess coordinates from 1-based indexing to
 * 0-based indexing used internally by the board. It returns a dynamically allocated array
 * containing the converted coordinates.
 *
 * @param guess A pointer to the Guess structure containing the user's guess coordinates.
 * @param board A pointer to the Board structure to which the coordinates will be applied.
 * @return A dynamically allocated array of two unsigned shorts representing the converted coordinates.
 */

unsigned short* convertGuessCoords(Guess* guess, Board* board)
{
    unsigned short h = guess->hor;
    unsigned short v = guess->ver;
    unsigned short* r_val = malloc(2 * sizeof(unsigned short));
    r_val[0] = h;
    r_val[1] = v;

    printf("Converted Guess: Hor: %hu, Ver: %hu\n", h, v); // Debugging

    return r_val;
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
 * @return true if the guess hits a bomb, false otherwise.
 */

bool takeTurn(Board* bombB, Board* flagB)
{
    Guess* guess = createGuess(bombB->hor, bombB->ver);
    if (guess == NULL) {
        return false;
    }

    unsigned short guessHor = guess->hor;
    unsigned short guessVer = guess->ver;

    // Additional check to ensure coordinates are valid
    if (guessHor >= bombB->hor || guessVer >= bombB->ver) {
        printf("Invalid coordinates: %hu,%hu\n", guessHor, guessVer);
        freeGuess(guess);
        return false;
    }

    // Print the corresponding row of the board
    printf("Guess coordinates: %hu,%hu\n", guessHor, guessVer);
    printf("Board row at %hu: ", guessHor);
    for (int i = 0; i < bombB->ver; i++) {
        printf("%d ", bombB->data[guessHor][i]);
    }
    printf("\n");

    printf("Bomb value: %d\n", bombB->data[guessHor][guessVer]);

    bool isBomb = (bombB->data[guessHor][guessVer] == 9);

    freeGuess(guess);
    return isBomb;
}

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

    Board* bombBoard = createBinaryBoard(hor, ver, bomb_count);
    Board* maskBoard = createBoard(hor, ver);
    Board* flagBoard = createBoard(hor, ver);

    populateNumberReferences(bombBoard);

    // DEBUG LINE
    printBoard(bombBoard);

    bool gameOver = false;
    while (!gameOver) {
        gameOver = takeTurn(bombBoard, flagBoard);
    }

    freeBoard(bombBoard);
    freeBoard(maskBoard);
    freeBoard(flagBoard);

    printf("Game Over!\n");
    return 0;
}
