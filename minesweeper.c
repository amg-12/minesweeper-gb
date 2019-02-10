#include <gb/gb.h>
#include <time.h>
#include <rand.h>
#include "alpha.c"

#define WIDTH 20
#define HEIGHT 18
#define MINES 25

#define HIDDEN 0
#define HIDDEN_MINE 1
#define FLAGGED 2
#define FLAGGED_MINE 3
#define REVEALED 4

UBYTE screen[WIDTH * HEIGHT];
#define BOARD(i, j) (screen[i + (WIDTH * j)])

#define P(button) (joypad() & button)

#define UPDATE_SCREEN set_bkg_tiles(0, 0, WIDTH, 32, screen);

void plantMines(UBYTE number) {

	UBYTE i;
	UWORD x;

	for (i = 0; i < number; i++) {
		do {
			x = randw();
			x = x % (WIDTH * HEIGHT);
		} while (BOARD(x, 0) == HIDDEN_MINE);
		BOARD(x, 0) = HIDDEN_MINE;
	}

}

void main() {

	//UBYTE i, j;

	waitpad(0xFF);
	initrand(clock());

	DISPLAY_ON;
	SHOW_BKG;

	set_bkg_data(0, 63, alpha);

	plantMines(MINES);

	UPDATE_SCREEN;

	while (1) {

	}

}