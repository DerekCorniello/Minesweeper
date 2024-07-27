
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

Guess* createGuess()
{
    printf("Enter 'F' or 'D' (Flag or Destroy), followed by coordinates, starting at 1,1 from the bottom left.\n");
    printf("Ex: F 1,1 -> flags the bottom left hand corner\n");

    unsigned short gVer;
    unsigned short gHor;
    char gMode;
    scanf(" %c %hu,%hu", &gMode, &gHor, &gVer);

    Guess* guess = (Guess*)malloc(sizeof(Guess));
    if (guess == NULL) {
        fprintf(stderr, "Failed to allocate memory for guess structure\n");
        return NULL;
    }

    guess->hor = gHor - 1;
    guess->ver = gVer - 1;
    guess->mode = gMode;

    printf("User Guess: Mode: %c, Hor: %hu, Ver: %hu\n", gMode, gHor, gVer); // Debugging

    return guess;
}

void freeGuess(Guess* g)
{
    free(g);
}

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

Board* makeBinBoard(unsigned short hor, unsigned short ver, unsigned int bomb_count)
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

void generateUserBoard(Board* board)
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

bool takeTurn(Board* bombB)
{
    Guess* guess = createGuess();
    if (guess == NULL) {
        return false;
    }

    unsigned short* guessCoords = convertGuessCoords(guess, bombB);

    if (guessCoords[0] >= bombB->hor || guessCoords[1] >= bombB->ver) {
        printf("Invalid coordinates.\n");
        free(guessCoords);
        freeGuess(guess);
        return false;
    }

    printf("Guess coordinates: %hu,%hu\n", guessCoords[0], guessCoords[1]);

    // Print the corresponding row of the board
    printf("Board row at %hu: ", guessCoords[0]);
    for (int i = 0; i < bombB->ver; i++) {
        printf("%d ", bombB->data[guessCoords[0]][i]);
    }
    printf("\n");

    printf("Bomb value: %d\n", bombB->data[guessCoords[0]][guessCoords[1]]);

    bool isBomb = (bombB->data[guessCoords[0]][guessCoords[1]] == 9);

    free(guessCoords);
    freeGuess(guess);
    return isBomb;
}

int main()
{
    printf("Welcome to Minesweeper.\n");
    printf("Please type the dimension of the board as two comma-separated values (EX: 10,5. Limit is 65535,65535):\n");

    unsigned short hor, ver;
    if (scanf("%hu,%hu", &hor, &ver) != 2) {
        printf("Invalid input. Please enter two integer values separated by a comma.\n");
        return 1;
    }

    if (hor <= 0 || ver <= 0 || hor > 65535 || ver > 65535) {
        printf("Dimensions must be positive integers between 1 and 65535.\n");
        return 1;
    }

    unsigned int bomb_count;
    printf("Please type the amount of bombs from 1 to %d.\n", (hor * ver - 1));
    if (scanf("%d", &bomb_count) != 1 || bomb_count < 1 || bomb_count > (hor * ver - 1)) {
        printf("Invalid bomb count. It must be a positive integer between 1 and %d.\n", (hor * ver - 1));
        return 1;
    }

    Board* bombBoard = makeBinBoard(hor, ver, bomb_count);
    Board* guessBoard = createBoard(hor, ver);

    generateUserBoard(bombBoard);

    // DEBUG LINE
    printBoard(bombBoard);

    bool gameOver = false;
    while (!gameOver) {
        gameOver = takeTurn(bombBoard);
    }

    freeBoard(bombBoard);
    freeBoard(guessBoard);

    printf("Game Over!\n");
    return 0;
}
