/* width of each charcter including inter-character space */
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define NUM_CHARS(X) (sizeof(X) - 1)

/* This is the highest value that x can be without the end of the text
 * going farther than the right side of the screen. We add one because
 * there will be a 1 pixel space at the end of the last character.
 * WIDTH and HEIGHT are defined in the Arduboy library. */
#define X_MAX(X) (WIDTH - (NUM_CHARS(X) * CHAR_WIDTH) + 1)
#define Y_MAX(X) (HEIGHT - CHAR_HEIGHT)

/* vi: set syn=c ts=8 sts=8 sw=8 noet cindent: */
