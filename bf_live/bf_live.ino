#include "Arduboy.h"
#include "bf_live.h"
#include "bf_interpret.h"

Arduboy arduboy;
bool ok;
struct bfstate state;

void setup(void)
{
	arduboy.begin();
	arduboy.setFrameRate(15);

	bfstate_init(&state);
	ok = is_balanced(hi_program) && interpret(&state, hi_program);
}

void loop(void)
{
	if (!(arduboy.nextFrame()))
		return;

	arduboy.clear();
	arduboy.setCursor(0, 0);

	if (ok)
		arduboy.print(state.output);
	else
		arduboy.print(":(");

	arduboy.display();
}

/* vi: set syn=arduino ts=8 sts=8 sw=8 noet cindent: */
