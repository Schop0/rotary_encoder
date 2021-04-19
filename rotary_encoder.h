#include <stdbool.h>

typedef enum
{
	LEFT,
	RIGHT,
	UNINITIALISED
} direction_t;

// Arguments: functions to retrieve I/O pin states
void rotary_init(
		bool (*a)(void),
		bool (*b)(void)
	);

// Rotary encoder statemachine update
// Call at least as often as I/O pin changes occur
// Preferably on an interrupt basis as polling may be less reliable
void rotary_update(void);

// Function to handle rotation events
// Implement this function in a user file
void rotary_callback(direction_t direction);

#ifdef DEBUG
// String representation of the current state
const char * getStateName(void);
#endif
