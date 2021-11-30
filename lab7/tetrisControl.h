#include <stdbool.h>
#include <stdint.h>

#ifndef TETRISCONTROL_H_
#define TETRISCONTROL_H_

// Used to init the state machine. Always provided though it may not be
// necessary.
void tetrisControl_init();

// Standard tick function.
void tetrisControl_tick();

#endif /* TETRISCONTROL_H_ */
