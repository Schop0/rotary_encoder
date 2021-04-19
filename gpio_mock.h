#ifndef GPIO_MOCK_H
#define GPIO_MOCK_H

#include <stdbool.h>

static bool a = false;
static bool b = false;

static inline bool getA(void)       { return a; }
static inline bool getB(void)       { return b; }
static inline bool setA(bool value) { return a = value; }
static inline bool setB(bool value) { return b = value; }
static inline bool toggleA(void)    { return setA(!getA()); }
static inline bool toggleB(void)    { return setB(!getB()); }

#endif /* GPIO_MOCK_H */
