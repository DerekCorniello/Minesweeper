#include "../src/minesweeper.h"
#include <assert.h>
#include <stdlib.h>

void testGuess()
{
    // tests wrong guess mode
    Guess* g = createGuess('x', 1, 1);
    assert(g == NULL);

    // tests large inputs
    // do i really need this?
    g = createGuess('F', MAX_HOR + 1, MAX_VER + 1);
    assert((g == NULL) || (g->hor != MAX_VER) || (g->ver != MAX_VER));

    // test good input
    g = createGuess('D', 2, 2);
    assert(g != NULL);
    assert(g->mode == 'D');
    // change indexing
    assert(g->ver == 1);
    assert(g->hor == 1);

    // test edge case
    g = createGuess('F', MAX_HOR, MAX_VER);
    assert(g != NULL);
    assert(g->mode == 'F');
    assert(g->ver == MAX_HOR - 1);
    assert(g->hor == MAX_VER - 1);

    freeGuess(g);
}

void testBoard()
{
    // test too small board
    Board* b = createBoard(1, 1);
    assert(b == NULL);

    // test too larbe board
    b = createBoard(MAX_HOR + 1, MAX_VER + 1);
    assert((b == NULL) || (b->hor != MAX_VER + 1) || (b->ver != MAX_VER + 1));

    // test edge case
    b = createBoard(MAX_HOR, MAX_VER);
    assert(b->ver == MAX_HOR);
    assert(b->hor == MAX_VER);

    // test good input
    b = createBoard(5, 5);
    assert(b->ver == 5);
    assert(b->hor == 5);

    // tests too many bombs
    b = createBombBoard(2, 2, 5);
    assert(b == NULL);

    // tests middle is empty, tests 24 bombs
    b = createBombBoard(5, 5, 24);
    assert(b != NULL);
    int count = 0;
    for (int i = 0; i < b->hor; i++) {
        for (int j = 0; j < b->ver; j++) {
            if (b->data[i][j] == 9) {
                count++;
            }
        }
    }
    assert(count == 24);
    assert(b->data[(b->hor / 2) - 1][(b->ver / 2) - 1] != 9);
}

int main()
{
    testGuess();
    testBoard();
    return 0;
}
