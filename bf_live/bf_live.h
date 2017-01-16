#include "Arduboy2.h"

/* width of each charcter including inter-character space */
#define CHAR_WIDTH	6
#define CHAR_HEIGHT	8
#define SCREEN_WIDTH	0x80
#define SCREEN_HEIGHT	0x40
#define LINE_CHARS (SCREEN_WIDTH / CHAR_WIDTH)

#define NUM_CHARS(X) (sizeof(X) - 1)

/* This is the highest value that x can be without the end of the text
 * going farther than the right side of the screen. We add one because
 * there will be a 1 pixel space at the end of the last character.
 * WIDTH and HEIGHT are defined in the Arduboy library. */
#define X_MAX(X) (WIDTH - (NUM_CHARS(X) * CHAR_WIDTH) + 1)
#define Y_MAX(X) (HEIGHT - CHAR_HEIGHT)

struct buttons {
	bool a, b, right, left, down, up;
};

struct buttons get_buttons(Arduboy2 *arduboy) {
	struct buttons bs;

	bs.a = arduboy->pressed(A_BUTTON);
	bs.b = arduboy->pressed(B_BUTTON);
	bs.right = arduboy->pressed(RIGHT_BUTTON);
	bs.left = arduboy->pressed(LEFT_BUTTON);
	bs.down = arduboy->pressed(DOWN_BUTTON);
	bs.up = arduboy->pressed(UP_BUTTON);

	return bs;
}

struct buttons calc_just_pressed(struct buttons cur, struct buttons last)
{
	struct buttons bs;

	bs.a = cur.a && !last.a;
	bs.b = cur.b && !last.b;
	bs.right = cur.right && !last.right;
	bs.left = cur.left && !last.left;
	bs.down = cur.down && !last.down;
	bs.up = cur.up && !last.up;

	return bs;
}

/* Wrap the Arduino tone() function so that the pin doesn't have to be
 * specified each time. Also, don't play if audio is set to off. */
void tone(unsigned int frequency, unsigned long duration, Arduboy2 *arduboy)
{
  if (arduboy->audio.enabled())
    tone(PIN_SPEAKER_1, frequency, duration);
}

/* print a null-terminated string on the character matrix, aligned left. returns
 * the line after the last line printed on. */
unsigned int matrix_print(char *s, unsigned int y0, unsigned int max_lines, Arduboy2 *arduboy)
{
	/* set up a string buffer for a single screen-line of text */
	char buf[LINE_CHARS + 1];
	buf[LINE_CHARS] = 0;

	/* either fill or flush the buffer with each character in the string */
	int hpos = 0, vpos = 0, i = 0;
	char c;
	while (1) {
		if (max_lines && vpos == max_lines)
			break;

		c = *s++;
		if (c)
			buf[hpos++] = c;

		/* do we need to flush the buffer? */
		if (hpos == LINE_CHARS || !c) {
			buf[hpos] = 0;
			arduboy->setCursor(0, y0 + vpos * CHAR_HEIGHT);
			arduboy->print(&buf[0]);
			hpos = 0;
			vpos++;
		}

		if (!c)
			break;
	}

	return vpos;
}

void invert_rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Arduboy2 *arduboy)
{
	for (int x = x0; x < x1; x++)
		for (int y = y0; y < y1; y++)
			arduboy->drawPixel(x, y, !arduboy->getPixel(x, y));
}

/* vi: set syn=c ts=8 sts=8 sw=8 noet cindent: */
