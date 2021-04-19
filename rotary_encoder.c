#include "rotary_encoder.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// Globals
static bool (*getA)(void) = NULL;
static bool (*getB)(void) = NULL;
static void (*state)(void) = NULL;
static void (*state_next)(void) = NULL;
static direction_t direction = UNINITIALISED;

// Prototypes

static void Q1(void);
static void Q2(void);
static void Q3(void);
static void Q4(void);

// Private

static void Q_unknown(void)
{
	if (getA())
	{
		if (getB())
			state_next = Q3;
		else
			state_next = Q2;
	}
	else
	{
		if (getB())
			state_next = Q4;
		else
			state_next = Q1;
	}
}

static void Q1(void)
{
	state_next = state;
	if ( getA()) state_next = Q2;
	if ( getB()) state_next = Q4;
}

static void Q2(void)
{
	state_next = state;
	if (!getA()) state_next = Q1;
	if ( getB()) state_next = Q3;
}

static void Q3(void)
{
	state_next = state;
	if (!getA()) state_next = Q4;
	if (!getB()) state_next = Q2;
}

static void Q4(void)
{
	state_next = state;
	if ( getA()) state_next = Q3;
	if (!getB()) state_next = Q1;
}

static void rotate(direction_t direction_next)
{
	if (direction != direction_next)
	{
		// Ignore direction changes, these may be transient
		direction = direction_next;
	}
	else
	{
		// Act after a full quadrant of movement in the same direction
		rotary_callback(direction);
	}
}

static void transition(
		void (*old)(void),
		void (*new)(void)
	)
{
	if (old == new || old == Q_unknown)
		return;

	if ((old == Q1 && new == Q2) ||
		(old == Q2 && new == Q3) ||
		(old == Q3 && new == Q4) ||
		(old == Q4 && new == Q1) )
	{
		rotate(RIGHT);
	}
	else
	if ((old == Q1 && new == Q4) ||
		(old == Q2 && new == Q1) ||
		(old == Q3 && new == Q2) ||
		(old == Q4 && new == Q3) )
	{
		rotate(LEFT);
	}
	else
	{
		// Illegal transition
		state_next = Q_unknown;
	}
}

// Public

void rotary_update(void)
{
	if (state) state();

	transition(state, state_next);

	state = state_next;
}

void rotary_init(
		bool (*a)(void),
		bool (*b)(void)
	)
{
	if ((NULL == a) || (NULL == b))
	{
		return;
	}

	getA = a;
	getB = b;
	state = Q_unknown;

	rotary_update();
}

void __attribute__((weak)) rotary_callback(direction_t direction)
{
	(void)direction; // Supress unused warning
	
	// This function should not be modified,
	// it can be implemented in a user file.
}

#ifdef DEBUG
const char * getStateName(void)
{
	if (state == Q1) return "Q1";
	if (state == Q2) return "Q2";
	if (state == Q3) return "Q3";
	if (state == Q4) return "Q4";

	return "Unknown";
}
#endif
