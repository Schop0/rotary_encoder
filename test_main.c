#include "rotary_encoder.h"
#include "gpio_mock.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE 255

void rotary_callback(direction_t direction)
{
	static int rotations = 0;

	switch (direction)
	{
		case RIGHT : rotations++; break;
		case LEFT  : rotations--; break;
		default : fputs("Warning: Illegal rotaton\n", stderr) ;
	}
	printf("Total clockwise rotation:%3d qadrants\n", rotations);
}

static inline const char * BOOL(bool b)
{
	return b ? "true" : "false";
}

static bool command(char cmd)
{
	switch (toupper(cmd))
	{
		case 'A':
			toggleA();
		break;
		case 'B':
			toggleB();
		break;
		default :
			fputs("Error: Command not found\n", stderr);
			return false;
	}
	return true;
}

#ifdef DEBUG
static void printstate(void)
{
	printf("A = %5s, B = %5s\n", BOOL(a), BOOL(b));
	printf("State: %s\n", getStateName());
}
#endif

int main (int argc, char **argv)
{
	char buf[SIZE] = {0};
	
	rotary_init(getA, getB);

#ifdef DEBUG
	printstate();
#endif

	while (fgets(buf, SIZE, stdin))
	{
		command(buf[0]);
		rotary_update();
#ifdef DEBUG
		printstate();
#endif
	}
}
