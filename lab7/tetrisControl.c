#include <stdbool.h>
#include <stdint.h>
#include "tetrisControl.h"
#include "tetrisDisplay.h"
#include "display.h"
#include "buttons.h"

#define FALL_TIME 5
#define TETRIS_TEXT_SIZE 5
#define TOUCH_TEXT_SIZE 2
#define LOSE_TEXT_SIZE 4
#define SCORE_TEXT_SIZE 1
#define START_MSG "TETRIS"
#define START_TOUCH_MSG "Touch to start game"
#define RESTART_TOUCH_MSG "Touch to start a new game"
#define LOSE_MSG "HA, YOU LOSE"
#define SCORE_MSG "SCORE"
#define X_CURSOR DISPLAY_WIDTH / 2 - 90
#define Y_CURSOR DISPLAY_HEIGHT / 2 - 30
#define X_TOUCH_CURSOR X_CURSOR - 20
#define Y_TOUCH_CURSOR Y_CURSOR + 60
#define X_LOSE_CURSOR X_CURSOR - 50
#define X_LTOUCH_CURSOR X_LOSE_CURSOR - 10
#define X_SCORE_CURSOR DISPLAY_WIDTH * 2/3 + 50
#define Y_SCORE_CURSOR DISPLAY_HEIGHT * 2/3

static enum tetrisControl_State_t { // Tetris control state machine
  init_st,
  start_msg_st,
  get_shape_st,
  falling_st,
  resting_st,
  check_row_st,
  lose_st
} currentState;

void tetrisControl_drawStartMsg(bool erase) {
    if (erase) {
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextSize(TETRIS_TEXT_SIZE);
    display_setTextColor(DISPLAY_BLACK);
    display_println(START_MSG);
    display_setCursor(X_TOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(START_TOUCH_MSG);
    }
    else {
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextSize(TETRIS_TEXT_SIZE);
    display_setTextColor(DISPLAY_WHITE);
    display_println(START_MSG);
    display_setCursor(X_TOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(START_TOUCH_MSG);
    }
}

void tetrisControl_drawLoseMsg(bool erase) {
    if (erase) {
    display_setCursor(X_LOSE_CURSOR, Y_CURSOR);
    display_setTextSize(LOSE_TEXT_SIZE);
    display_setTextColor(DISPLAY_BLACK);
    display_println(LOSE_MSG);
    display_setCursor(X_LTOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(RESTART_TOUCH_MSG);
    }
    else {
    display_setCursor(X_LOSE_CURSOR, Y_CURSOR);
    display_setTextSize(LOSE_TEXT_SIZE);
    display_setTextColor(DISPLAY_WHITE);
    display_println(LOSE_MSG);
    display_setCursor(X_LTOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(RESTART_TOUCH_MSG);
    }
}

void tetrisControl_drawScore() {
    display_setCursor(X_SCORE_CURSOR, Y_SCORE_CURSOR);
    display_setTextSize(SCORE_TEXT_SIZE);
    display_setTextColor(DISPLAY_WHITE);
    display_println(SCORE_MSG);
}

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
            if (!display_isTouched()) {
                currentState = start_msg_st;
            }
            else {
                tetrisControl_drawStartMsg(true);
                currentState = get_shape_st;
            }
            break;
        case get_shape_st:
            currentState = falling_st;
            break;
        case falling_st:
            /* if (!verticalCollision) {
                currentState= falling_st;
            }
            else if (fallTime == FALL_TIME) {

            }
            else {
                currentState = resting_st;
            } */
            break;
        case resting_st:
            currentState = check_row_st;
            break;
        case check_row_st:
            /* if (screen_isFull()) {
                display_clearScreen();
                currentState = lose_st;
            }
            else {
                Update the score
                currentState = get_shape_st;
            } */
            break;
        case lose_st:
            if (display_isTouched()) {
                tetrisControl_drawLoseMsg(true);
                currentState = start_msg_st;
            }
            else {
                currentState = lose_st;
            }
            break;
    }

    switch(currentState) { //Action states
        case init_st:
            break;
        case start_msg_st:
            tetrisControl_drawStartMsg(false);
            break;
        case get_shape_st:
            //tetrisDisplay_init();
            //tetrisControl_displayScore();
            //makeShape();
            break;
        case falling_st:
            break;
        case resting_st:
            //stopShape();
            break;
        case check_row_st:
            break;
        case lose_st:
            tetrisControl_drawLoseMsg(false);
            //clearGameBoard();
            //clearScore();
            break;
    }
}
