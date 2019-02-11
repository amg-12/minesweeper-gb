#include <gb/gb.h>
#include <time.h>
#include <rand.h>
#include "alpha.c"

#include "C:\gbdk\libc\malloc.c" // gbdk is very broken

#define BLINK_INTERVAL 32
#define INPUT_DELAY 150

#define HIDDEN 0
#define HIDDEN_MINE 1
#define FLAGGED 2
#define FLAGGED_MINE 3
#define REVEALED 4

UBYTE width;
UBYTE height;
UBYTE mines;
UWORD widthheight;
UBYTE playerX = 0;
UBYTE playerY = 0;

UBYTE *board;
#define BOARD(i, j) (board[i + (width * j)])

#define P(button) (joypad() & button)

#define MOVE_SPRITE(sprite, x, y) move_sprite(sprite, (x + 1) * 8, (y + 2) * 8);

#define DISPLAY(source) set_bkg_tiles(0, 0, width, height, source);

void plantMines(UBYTE *board, UBYTE number) {

	UBYTE i;
	UWORD x;

	for (i = 0; i < number; i++) {
		do {
			x = randw();
			x = x % (widthheight);
		} while (board[x] == HIDDEN_MINE);
		board[x] = HIDDEN_MINE;
	}

}

void waitAndBlink(UBYTE sprite) {
	
	UBYTE blink = 1;

	while (!joypad()) {
		if (clock() % BLINK_INTERVAL == 0) {
			set_sprite_tile(sprite, (blink = !blink));
			delay(1);
		}
	}
	set_sprite_tile(sprite, 1);

}

void handleInput() {

	if (P(J_UP)) {
		playerY--;
	}
	if (P(J_DOWN)) {
		playerY++;
	}
	if (P(J_LEFT)) {
		playerX--;
	}
	if (P(J_RIGHT)) {
		playerX++;
	}

	MOVE_SPRITE(0, playerX, playerY);
	delay(INPUT_DELAY);

}

void main() {

	//UBYTE i, j;

	width = 20;
	height = 18;
	mines = 80;

	widthheight = width * height;

	board = malloc(widthheight * sizeof(UBYTE));

	waitpad(0xFF);
	initrand(clock());

	DISPLAY_ON;
	SHOW_BKG;
	SHOW_SPRITES;

	set_bkg_data(0, 63, alpha);

	set_sprite_tile(0, 1);
	MOVE_SPRITE(0, playerX, playerY);

	plantMines(board, mines);

	DISPLAY(board);

	while (1) {
		waitAndBlink(0);
		handleInput();
	}

}