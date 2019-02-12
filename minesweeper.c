#include <gb/gb.h>
#include <time.h>
#include <rand.h>
#include "assets/tiles.c"
#include "assets/selector.c"

#include "C:\gbdk\libc\malloc.c" // gbdk is very broken

#define BLINK_INTERVAL 32
#define INPUT_DELAY 150

#define HIDDEN 0
#define HIDDEN_MINE 1
#define FLAGGED 2
#define FLAGGED_MINE 3
#define REVEALED_MINE 4
#define REVEALED 6

UBYTE started;
UBYTE width;
UBYTE height;
UBYTE mines;
UWORD widthheight;
UBYTE playerX = 0;
UBYTE playerY = 0;

UBYTE *board;
#define BOARD(i, j) (board[i + (width * j)])

#define IS_EDGE(x, y) (x == -1 || x == width || y == -1 || y == height)

#define P(button) (joypad() & button)

#define MOVE_SPRITE(sprite, x, y) move_sprite(sprite, (x + 1) * 8, (y + 2) * 8);

#define DISPLAY(source) set_bkg_tiles(0, 0, width, height, source);

void plantMines() {

	UBYTE i;
	UWORD x;

	for (i = 0; i < mines; i++) {
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

reveal(UWORD x, UWORD y) {

	UBYTE i, j;
	UWORD k, l;
	UBYTE count = 0;

	if (BOARD(x, y) == HIDDEN_MINE) {
		BOARD(x, y) = REVEALED_MINE;
	}

	else if (BOARD(x, y) == HIDDEN) {
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				k = x + i - 1;
				l = y + j - 1;
				if (!IS_EDGE(k, l) && (BOARD(k, l) == HIDDEN_MINE || BOARD(k, l) == FLAGGED_MINE)) {
					count++;
				}
			}
		}
		BOARD(x, y) = REVEALED + count;
	}

	if (BOARD(x, y) == REVEALED) { // recursively reveal everything around a 0
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				k = x + i - 1;
				l = y + j - 1;
				if (!IS_EDGE(k, l) && BOARD(k, l) == HIDDEN) {
					reveal(k, l);
				}
			}
		}
	}

}

void flag(UWORD x, UWORD y) {
	
	switch (BOARD(x, y)) {
		case HIDDEN:
			BOARD(x, y) = FLAGGED;
			break;
		case FLAGGED:
			BOARD(x, y) = HIDDEN;
			break;
		case HIDDEN_MINE:
			BOARD(x, y) = FLAGGED_MINE;
			break;
		case FLAGGED_MINE:
			BOARD(x, y) = HIDDEN_MINE;
			break;
	}

}

void start() {

	UBYTE i, j;
	UWORD k, l;

	initrand(clock());

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			k = playerX + i - 1;
			l = playerY + j - 1;
			if (!IS_EDGE(k, l)) {
				BOARD(k, l) = HIDDEN_MINE;
			}
		}
	}

	plantMines();
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			k = playerX + i - 1;
			l = playerY + j - 1;
			if (!IS_EDGE(k, l)) {
				BOARD(k, l) = HIDDEN;
			}
		}
	}
	
	reveal(playerX, playerY);
	started = 1;

}

void handleInput() {

	if (P(J_UP) && playerY > 0) {
		playerY--;
	}
	if (P(J_DOWN) && playerY < height - 1) {
		playerY++;
	}
	if (P(J_LEFT) && playerX > 0) {
		playerX--;
	}
	if (P(J_RIGHT && playerX < width - 1)) {
		playerX++;
	}
	if (P(J_B) && started) {
		flag(playerX, playerY);
	}
	if (P(J_A) && started) {
		reveal(playerX, playerY);
	}
	if (P(J_A && !started)) {
		start();
	}

	MOVE_SPRITE(0, playerX, playerY);
	DISPLAY(board);
	delay(INPUT_DELAY);

}

void main() {

	started = 0;

	width = 20;
	height = 18;
	mines = 72;

	widthheight = width * height;

	board = malloc(widthheight * sizeof(UBYTE));

	DISPLAY_ON;
	SHOW_BKG;
	SHOW_SPRITES;

	set_bkg_data(0, 15, tiles);
	set_sprite_data(1, 1, selector);

	set_sprite_tile(0, 1);
	MOVE_SPRITE(0, playerX, playerY);

	DISPLAY(board);

	while (1) {

		waitAndBlink(0);
		handleInput();

	}

}