#include "Arduboy2.h"
#include "bf_live.h"
#include "bf_interpret.h"

#define PROG_SIZE (LINE_CHARS * 10)

Arduboy2 arduboy;
bool changed_since_a, changed_since_b, ok, reexecute;
char program[PROG_SIZE + 1] =	"++++++++++[>++++++++++<-]>++++.---.+++++++..+++.>++++++++[>++++<-]>+.";
unsigned int cursor;
struct bfstate state;
struct buttons buttons, buttons_last, just_pressed, just_released;
unsigned int x, y;

void setup(void)
{
	/* initialize variables */
	bfstate_init(&state);
	cursor = 0;
	changed_since_a = false;
	changed_since_b = false;
	reexecute = true;

	/* set up arduboy */
	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();
	arduboy.audio.begin();
	arduboy.setFrameRate(15);

	/* give a clean slate for programming (true) or use default program,
	 * but leave room for more (false) */
	bool clean_slate = arduboy.pressed(DOWN_BUTTON);

	for (int i = 0; i < PROG_SIZE; i++)
		if (clean_slate || !program[i])
			program[i] = ' ';

	/* set null byte at the end of the program */
	program[PROG_SIZE] = 0;
}

void loop(void) {
	if (!(arduboy.nextFrame()))
		return;

	/* run the program if the source has changed */
	if (reexecute) {
		bfstate_init(&state);
		ok = is_balanced(&program[0]) && interpret(&program[0], &state);
		reexecute = false;
	}

	/* get button status */
	buttons_last = buttons;
	buttons = get_buttons(&arduboy);
	just_pressed = calc_just_pressed(buttons, buttons_last);
	just_released = calc_just_pressed(buttons_last, buttons);

	/* when A is held */
	if (buttons.a && !buttons.b) {
		if (just_pressed.right) {
			program[cursor++] = '>';
			changed_since_a = true;
		} else if (just_pressed.left) {
			program[cursor++] = '<';
			changed_since_a = true;
		} else if (just_pressed.down) {
			program[cursor++] = '-';
			changed_since_a = true;
		} else if (just_pressed.up) {
			program[cursor++] = '+';
			changed_since_a = true;
		}

	/* when B is held */
	} else if (buttons.b && !buttons.a) {
		if (just_pressed.right) {
			program[cursor++] = ']';
			changed_since_b = true;
		} else if (just_pressed.left) {
			program[cursor++] = '[';
			changed_since_b = true;
		} else if (just_pressed.up) {
			program[cursor++] = '.';
			changed_since_b = true;
		}

	} else {
		if (just_pressed.right)
			cursor++;
		if (just_pressed.left && cursor)
			cursor--;
		if (just_pressed.down)
			cursor += LINE_CHARS;
		if (just_pressed.up && cursor >= LINE_CHARS)
			cursor -= LINE_CHARS;

		cursor %= 5 * LINE_CHARS;
	}

	if (just_released.a) {
		if (!changed_since_a)
			program[cursor++] = ' ';
		changed_since_a = false;
		reexecute = true;
	}

	if (just_released.b) {
		if (!changed_since_b)
			program[cursor++] = ' ';
		changed_since_b = false;
		reexecute = true;
	}

	arduboy.clear();

	/* draw the editor */
	matrix_print(&program[0], 0, 5, &arduboy);

	/* draw the cursor */
	x = (cursor % LINE_CHARS) * CHAR_WIDTH;
	y = (cursor / LINE_CHARS) * CHAR_HEIGHT;
	if (!buttons.a && !buttons.b)
		invert_rect(x, y, x + CHAR_WIDTH - 1, y + CHAR_HEIGHT - 1, &arduboy);

	/* separator between editor and output */
	arduboy.drawLine(0x10, 0x2b, SCREEN_WIDTH - 1 - 0x10, 0x2d, 1);
	arduboy.drawLine(0x10, 0x2d, SCREEN_WIDTH - 1 - 0x10, 0x2b, 1);

	/* draw the output section */
	matrix_print(ok? state.output : "err", 0x30, 2, &arduboy);
	arduboy.display();
}


/* vi: set syn=arduino ts=8 sts=8 sw=8 noet cindent: */
