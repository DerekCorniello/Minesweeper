#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdbool.h>

#define MAX_HOR 65535
#define MAX_VER 65535

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

Guess* createGuess(char gMode, unsigned short gVer, unsigned short gHor);
void freeGuess(Guess* g);

Board* createBoard(unsigned short hor, unsigned short ver);
void freeBoard(Board* board);

Board* createBinaryBoard(unsigned short hor, unsigned short ver, unsigned int bomb_count);
void populateNumberReferences(Board* board);

void printBoard(Board* board);
void printTotalBoard(Board* bomb, Board* flag, Board* usermask);

unsigned short* convertGuessCoords(Guess* guess, Board* board);

void destroySpacesAround(Board* bombB, Board* flagB, Board* maskB, unsigned short hor, unsigned short ver);

bool takeTurn(Board* bombB, Board* flagB, Board* maskB);

bool compBoards(unsigned int bombCount, Board* bomb, Board* flag);

#endif
