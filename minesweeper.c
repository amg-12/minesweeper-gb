#include <gb/gb.h>
#include <time.h>
#include <rand.h>
#include "alpha.c"

#include "C:\gbdk\libc\malloc.c" // gbdk is very broken

#define WIDTH 20
#define HEIGHT 18
#define MINES 80

#define HIDDEN 0
#define HIDDEN_MINE 1
#define FLAGGED 2
#define FLAGGED_MINE 3
#define REVEALED 4

UBYTE *board;
#define BOARD(i, j) (board[i + (WIDTH * j)])

#define P(button) (joypad() & button)

#define DISPLAY(source) set_bkg_tiles(0, 0, WIDTH, HEIGHT, source);

void plantMines(UBYTE *board, UBYTE number) {

	UBYTE i;
	UWORD x;

	for (i = 0; i < number; i++) {
		do {
			x = randw();
			x = x % (WIDTH * HEIGHT);
		} while (board[x] == HIDDEN_MINE);
		board[x] = HIDDEN_MINE;
	}

}

void main() {

	//UBYTE i, j;
	
	board = malloc(WIDTH * HEIGHT * sizeof(UBYTE));

	waitpad(0xFF);
	initrand(clock());

	DISPLAY_ON;
	SHOW_BKG;
	SHOW_SPRITES;

	set_bkg_data(0, 63, alpha);

	plantMines(board, MINES);

	DISPLAY(board);

	while (1) {

	}

}