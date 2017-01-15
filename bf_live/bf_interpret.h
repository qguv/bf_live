#define TAPE_SIZE	256
#define OUTPUT_SIZE	256
#define MAX_NESTS	256

const char *hi_program = "++++ ++++ ++ [> ++++ ++++ ++ <-]> ++++ . + .";

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

		if (nesting_level < 0)
			return false;
	}
	return (nesting_level == 0);
}

/* before calling, verify that the program is balanced with is_balanced */
static bool interpret(struct bfstate *s, const char *program)
{
	const char *return_to[MAX_NESTS];
	unsigned int cur_nest = 0;

	char c;
	while ((c = *program++)) {
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
			if (s->tape_cursor == TAPE_SIZE - 1)
				return false;

			s->tape_cursor++;
			break;

		/* move tape left by one */
		case '<':

			/* accessing a positive tape element? */
			if (s->tape_cursor == 0)
				return false;

			s->tape_cursor--;
			break;

		/* begin loop */
		case '[':

			/* enough nest buffer space? */
			if (cur_nest == MAX_NESTS - 1)
				return false;

			return_to[cur_nest] = program;
			cur_nest++;
			break;

		/* end loop */
		case ']':

			/* balanced? safe to deref program pointer? */
			if (cur_nest == 0)
				return false;

			if (s->tape[s->tape_cursor] != 0)
				program = return_to[cur_nest - 1] - 1;
			else
				cur_nest--;
			break;

		/* print character */
		case '.':

			/* enough space in the output buffer? */
			if (s->output_cursor == OUTPUT_SIZE - 1)
				return false;

			s->output[s->output_cursor] = (char) s->tape[s->tape_cursor];
			s->output_cursor++;
			break;

		/* input character */
		/* TODO: unimplemented */
		case ',':

		/* everything else is a comment */
		default:
			;
			break;
		}
	}
	return true;
}

/* vi: set syn=c ts=8 sts=8 sw=8 noet cindent: */
