
#include <stdbool.h>
#include <stdint.h>
#include "tetrisControl.h"

static enum tetrisControl_State_t { // Tetris control state machine
  init_st,
  start_msg_st,
  get_shape_st,
  falling_st,
  resting_st,
  check_row_st,
  lose_st
} currentState;

// Used to init the state machine. Always provided though it may not be
// necessary.
void tetrisControl_init() {
    currentState = init_st;
}

// Standard tick function.
void tetrisControl_tick() {
    switch(currentState) { //Transistion states
        case init_st:
            break;
        case start_msg_st:
            break;
        case get_shape_st:
            break;
        case falling_st:
            break;
        case resting_st:
            break;
        case check_row_st:
            break;
        case lose_st:
            break;
    }

    switch(currentState) { //Action states
        case init_st:
            break;
        case start_msg_st:
            break;
        case get_shape_st:
            break;
        case falling_st:
            break;
        case resting_st:
            break;
        case check_row_st:
            break;
        case lose_st:
            break;
    }
}
