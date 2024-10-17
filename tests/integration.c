#include "../src/minesweeper.h"
#include <assert.h>

void testBoardOperations()
{

    // Test dimensions and bomb count
    unsigned short hor = 5;
    unsigned short ver = 5;
    unsigned int bomb_count = 3;

    // Create the bomb board and validate
    Board* bombBoard = createBombBoard(hor, ver, bomb_count);
    assert(bombBoard != NULL);

    // Populate the number references on the bomb board
    populateNumberReferences(bombBoard);

    // Create the flag and mask boards
    Board* flagBoard = createBoard(hor, ver);
    assert(flagBoard != NULL);
    Board* maskBoard = createBoard(hor, ver);
    assert(maskBoard != NULL);

    // Simulate a turn: Flag a safe cell and check the game state
    Guess* guess = createGuess('F', 2, 2);
    assert(guess != NULL);
    maskBoard->data[guess->hor][guess->ver] = 1;
    flagBoard->data[guess->hor][guess->ver] = 1;
    freeGuess(guess);

    // Verify that the flag was placed correctly
    assert(flagBoard->data[1][1] == 1);
    assert(maskBoard->data[1][1] == 1);

    // Simulate a turn: Destroy a cell and check the game state
    guess = createGuess('D', 3, 3);
    assert(guess != NULL);
    bool hitBomb = takeTurn(bombBoard, flagBoard, maskBoard);
    assert(!hitBomb); // Ensure the guess didn't hit a bomb
    freeGuess(guess);

    // Verify that the mask board has been updated correctly
    assert(maskBoard->data[2][2] == 1);

    // Clean up
    freeBoard(bombBoard);
    freeBoard(flagBoard);
    freeBoard(maskBoard);
}
int main()
{
    testBoardOperations();
    return 0;
}
