#define TAPE_SIZE	128
#define OUTPUT_SIZE	128
#define MAX_NESTS	16

#ifndef DEBUG
void DEBUG(char*_,...){}
#endif

struct bfstate {
	unsigned int tape_cursor, output_cursor;
	unsigned char tape[TAPE_SIZE];
	char output[OUTPUT_SIZE];
};

void bfstate_init(struct bfstate *s)
{
	s->tape_cursor = s->output_cursor = 0;
	for (int i = 0; i < TAPE_SIZE; i++)
		s->tape[i] = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++)
		s->output[i] = 0;
}

bool is_balanced(const char *program)
{
	char nesting_level = 0;
	char c;
	while ((c = *program++)) {
		switch (c) {
		case '[':
			nesting_level++;
			break;
		case ']':
			nesting_level--;
			break;
		default:
			continue;
		}

		if (nesting_level < 0) {
			DEBUG("unmatched right brackets\n");
			return false;
		}
	}

	if (nesting_level != 0) {
		DEBUG("unmatched left brackets\n");
		return false;
	}

	return true;
}

/* before calling, verify that the program is balanced with is_balanced */
static bool interpret(const char *program, struct bfstate *s)
{
	const char *return_to[MAX_NESTS];
	unsigned int cur_nest = 0;

	char c;
	while ((c = *program++)) {
		DEBUG("Read '%c'\n", c);
		switch (c) {

		/* increment value at current tape slot */
		case '+':
			s->tape[s->tape_cursor]++;
			break;

		/* decrement value at current tape slot */
		case '-':
			s->tape[s->tape_cursor]--;
			break;

		/* move tape right by one */
		case '>':

			/* is tape long enough? */
			if (s->tape_cursor == TAPE_SIZE - 1) {
				DEBUG("tape isn't long enough\n");
				return false;
			}

			s->tape_cursor++;
			break;

		/* move tape left by one */
		case '<':

			/* accessing a positive tape element? */
			if (s->tape_cursor == 0) {
				DEBUG("accessing a negative tape element\n");
				return false;
			}

			s->tape_cursor--;
			break;

		/* begin loop */
		case '[':

			/* enough nest buffer space? */
			if (cur_nest == MAX_NESTS - 1) {
				DEBUG("not enough nest buffer space\n");
				return false;
			}

			return_to[cur_nest] = program;
			cur_nest++;
			break;

		/* end loop */
		case ']':

			/* balanced? safe to deref program pointer? */
			if (cur_nest == 0) {
				DEBUG("unsafe program pointer dereference (probably too many right-brackets)\n");
				return false;
			}

			/* if current cell isn't zero, loop back to the
			 * appropriate 'begin loop' token */
			if (s->tape[s->tape_cursor])
				program = return_to[cur_nest - 1];

			/* if current cell is zero, continue */
			else
				cur_nest--;

			break;

		/* print character */
		case '.':

			/* enough space in the output buffer? */
			if (s->output_cursor == OUTPUT_SIZE - 1) {
				DEBUG("Not enough space in output buffer");
				return false;
			}

			s->output[s->output_cursor] = (char) s->tape[s->tape_cursor];
			s->output_cursor++;
			break;

		/* everything else is a comment */
		default:
			;
			break;
		}
	}
	return true;
}

/* vi: set syn=c ts=8 sts=8 sw=8 noet cindent: */
