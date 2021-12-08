#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "tetrisControl.h"
#include "tetrisDisplay.h"
#include "display.h"
#include "buttons.h"
#include "switches.h"

#define STRING_LENGTH 10
#define FALL_TIME 7
#define INSTRUCT_TIMER 100
#define SCORE_TIMER 50
#define TETRIS_TEXT_SIZE 5
#define TOUCH_TEXT_SIZE 2
#define LOSE_TEXT_SIZE 4
#define SCORE_TEXT_SIZE 3
#define START_MSG "TETRIS"
#define START_TOUCH_MSG "Touch to start game"
#define RESTART_TOUCH_MSG "Touch to start a new game"
#define LOSE_MSG "HA, YOU LOSE"
#define SCORE_MSG "SCORE"
#define INSTRUCT_MSG "INSTRUCTIONS"
#define BTN0_KEY "BTN0 TO MOVE RIGHT"
#define BTN1_KEY "BTN1 TO MOVE LEFT"
#define BTN2_KEY "BTN2 TO ROTATE"
#define BTN3_KEY "BTN3 TO FALL FASTER"
#define SW0_KEY "SW0 TO END GAME"


#define X_CURSOR DISPLAY_WIDTH / 2 - 90
#define Y_CURSOR DISPLAY_HEIGHT / 2 - 30
#define X_TOUCH_CURSOR X_CURSOR - 20
#define Y_TOUCH_CURSOR Y_CURSOR + 60
#define X_LOSE_CURSOR X_CURSOR - 50
#define X_LTOUCH_CURSOR X_LOSE_CURSOR - 10
#define X_SCORE_CURSOR DISPLAY_WIDTH / 2 - 70
#define Y_SCORE_CURSOR DISPLAY_HEIGHT / 2 - 20
#define X_INSTRUCT_CURSOR DISPLAY_WIDTH / 2 - 110
#define Y_INSTRUCT_CURSOR DISPLAY_HEIGHT / 2 - 60
#define Y_BTN0_CURSOR Y_INSTRUCT_CURSOR + 20
#define Y_BTN1_CURSOR Y_BTN0_CURSOR + 20
#define Y_BTN2_CURSOR Y_BTN1_CURSOR + 20
#define Y_BTN3_CURSOR Y_BTN2_CURSOR + 20
#define Y_SW0_CURSOR Y_BTN3_CURSOR + 20

#define START_Y   0

Shape currentShape, nextShape;
Box board[NUM_COLS][NUM_ROWS];
Box placeable[MAX_PLACEABLE];

static enum tetrisControl_State_t { // Tetris control state machine
  init_st,
  start_msg_st,
  instruct_st,
  falling_st,
  check_row_st,
  lose_st,
  disp_score_st,
  lower_switch_st
} currentState;

static uint8_t instructTimer = 0, scoreTime = 0, fallCounter = 0;
static uint16_t startTimer = 0, currentScore = 0;
static bool buttonPressed = false;

void tetrisControl_drawStartMsg(bool erase) { //Draws and erases start message
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
    display_setTextColor(DISPLAY_RED);
    display_println("T");
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextColor(DISPLAY_GREEN);
    display_println(" E");
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextColor(0xFD00);
    display_println("  T");
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextColor(DISPLAY_CYAN);
    display_println("   R");
    display_setTextColor(DISPLAY_YELLOW);
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_println("    I");
    display_setCursor(X_CURSOR, Y_CURSOR);
    display_setTextColor(DISPLAY_MAGENTA);
    display_println("     S");
    display_setTextColor(DISPLAY_WHITE);
    display_setCursor(X_TOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(START_TOUCH_MSG);
    }
}

void tetrisControl_drawLoseMsg(bool erase) { //Draws and erases lose message
    uint16_t color = erase ? DISPLAY_BLACK : DISPLAY_WHITE;
    display_setTextColor(color);
    display_setCursor(X_LOSE_CURSOR, Y_CURSOR);
    display_setTextSize(LOSE_TEXT_SIZE);
    display_println(LOSE_MSG);
    display_setCursor(X_LTOUCH_CURSOR, Y_TOUCH_CURSOR);
    display_setTextSize(TOUCH_TEXT_SIZE);
    display_println(RESTART_TOUCH_MSG);
}

void tetrisControl_drawScore(bool erase) { //Displays the current score
    uint16_t color = erase ? DISPLAY_BLACK : DISPLAY_WHITE;
    display_setTextColor(color);
    display_setCursor(X_SCORE_CURSOR, Y_SCORE_CURSOR);
    display_setTextSize(SCORE_TEXT_SIZE);
    char stringMsg[STRING_LENGTH];
    sprintf(stringMsg, "%s: %d\n", SCORE_MSG, currentScore);
    display_println(stringMsg);
}

void tetrisControl_drawInstructions(bool erase) { //Draws and erases instructions
    uint16_t color = erase ? DISPLAY_BLACK : DISPLAY_WHITE;
    display_setTextColor(color);
    display_setCursor(X_INSTRUCT_CURSOR, Y_INSTRUCT_CURSOR);
    display_setTextSize(2);
    display_println(INSTRUCT_MSG);
    display_setCursor(X_INSTRUCT_CURSOR, Y_BTN0_CURSOR);
    display_println(BTN0_KEY);
    display_setCursor(X_INSTRUCT_CURSOR, Y_BTN1_CURSOR);
    display_println(BTN1_KEY);
    display_setCursor(X_INSTRUCT_CURSOR, Y_BTN2_CURSOR);
    display_println(BTN2_KEY);
    display_setCursor(X_INSTRUCT_CURSOR, Y_BTN3_CURSOR);
    display_println(BTN3_KEY);
    display_setCursor(X_INSTRUCT_CURSOR, Y_SW0_CURSOR);
    display_println(SW0_KEY);
}

void tetrisControl_inGameScore(bool erase) {
    uint16_t color = erase ? DISPLAY_BLACK : DISPLAY_WHITE;
    display_setTextSize(2);
    display_setCursor(260, DISPLAY_HEIGHT/2+10);
    display_setTextColor(color);
    char thisScore[STRING_LENGTH];
    sprintf(thisScore, "%d", currentScore);
    display_println(thisScore);
}

// Used to init the state machine. Always provided though it may not be
// necessary.
void tetrisControl_init() {
    currentState = init_st;
    buttons_init();
    switches_init();
}

// Standard tick function.
void tetrisControl_tick() {
    switch(currentState) { //Transistion states
        case init_st:
            tetrisControl_drawStartMsg(false);
            currentState = start_msg_st;
            break;
        case start_msg_st:
            if (!display_isTouched()) {
                currentState = start_msg_st;
            }
            else {
                srand(startTimer);
                tetrisControl_drawStartMsg(true);
                tetrisControl_drawInstructions(false);
                currentState = instruct_st;
            }
            break;
        case instruct_st:
            if (instructTimer >= INSTRUCT_TIMER) {
                tetrisControl_drawInstructions(true);
                instructTimer = 0;
                tetrisDisplay_init(board);
                tetrisControl_inGameScore(false);
                tetrisDisplay_getNextShape(&currentShape, board); 
                tetrisDisplay_drawShape(&currentShape);
                tetrisDisplay_getNextShape(&nextShape, board);
                tetrisDisplay_drawNextShape(&nextShape);
                currentState = falling_st;
            }
            else {
                currentState = instruct_st;
            }
            break;
        case falling_st:
            if (switches_read() & SWITCHES_SW0_MASK) {
                display_fillScreen(DISPLAY_BLACK);
                tetrisControl_drawScore(false);
                currentState = disp_score_st;
            }
            if (fallCounter >= FALL_TIME) {
                if (tetrisDisplay_bottomCollision(&currentShape, board)){
                    tetrisControl_inGameScore(true);
                    currentScore++;
                    tetrisControl_inGameScore(false);
                    fallCounter = 0;
                    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
                        board[currentShape.boxes[i].x_pos][currentShape.boxes[i].y_pos] = currentShape.boxes[i];
                        board[currentShape.boxes[i].x_pos][currentShape.boxes[i].y_pos].filled = true;
                    }
                    tetrisDisplay_updateCurrent(&nextShape, &currentShape, board);
                    tetrisDisplay_eraseNextShape(&nextShape);
                    //change seed
                    tetrisDisplay_getNextShape(&nextShape, board);
                    // tetrisDisplay_drawNextShape(&nextShape);
                    // tetrisDisplay_drawShape(&currentShape);
                    currentState = check_row_st;
                }
                else {
                    tetrisDisplay_fall(&currentShape, board);
                    fallCounter = 0;
                    currentState = falling_st;
                }
            }
            break;
        case check_row_st:
            if (switches_read() & SWITCHES_SW0_MASK) {
                display_fillScreen(DISPLAY_BLACK);
                tetrisControl_drawScore(false);
                currentState = disp_score_st;
            }
            else { // Put a pin in that one
                for (uint8_t i = 1; i < NUM_COLS - 1; i++) {
                    if (board[i][0].filled) {
                        display_fillScreen(DISPLAY_BLACK);
                        tetrisControl_drawLoseMsg(false);
                        currentState = lose_st;
                        break;
                    }
                }
                if (currentState!=lose_st) {
                    tetrisDisplay_drawNextShape(&nextShape);
                    tetrisDisplay_drawShape(&currentShape);
                    currentState = falling_st;
                }
            }
            break;
        case lose_st:
            if (display_isTouched()) {
                tetrisControl_drawLoseMsg(true);
                tetrisControl_drawScore(false);
                scoreTime = 0;
                currentState = disp_score_st;
            }
            else {
                currentState = lose_st;
            }
            break;
        case disp_score_st:
            if (scoreTime == SCORE_TIMER) {
                if(switches_read() & SWITCHES_SW0_MASK) {
                    tetrisControl_drawScore(true);
                    display_setCursor(X_CURSOR - 50, Y_SCORE_CURSOR);
                    display_setTextColor(DISPLAY_WHITE);
                    display_println("Lower the Switch");
                    scoreTime = 0;
                    currentScore = 0;
                    currentState = lower_switch_st;
                }
                else {
                    tetrisControl_drawScore(true);
                    scoreTime = 0;
                    currentScore = 0;
                    tetrisControl_drawStartMsg(false);
                    currentState = start_msg_st;
                }
            }
            else {
                currentState = disp_score_st;
            }
            break;
        case lower_switch_st:
            if (!switches_read() & SWITCHES_SW0_MASK) {
                display_setCursor(X_CURSOR - 50, Y_SCORE_CURSOR);
                display_setTextColor(DISPLAY_BLACK);
                display_println("Lower the Switch");
                tetrisControl_drawStartMsg(false);
                currentState = start_msg_st;
            }
    }

    switch(currentState) { //Action states
        case init_st:
            break;
        case start_msg_st:
            startTimer++;
            break;
        case instruct_st:
            instructTimer++;
            break;
        case falling_st:
            if (buttons_read() & BUTTONS_BTN3_MASK) {
                fallCounter = fallCounter + 4;
            }
            if (buttons_read() & BUTTONS_BTN0_MASK) {
                if (!buttonPressed) {
                    tetrisDisplay_moveShape(&currentShape, board, true);
                    buttonPressed = true;
                }
            }
            else if (buttons_read() & BUTTONS_BTN1_MASK) {
                if (!buttonPressed) {
                    tetrisDisplay_moveShape(&currentShape, board, false);
                    buttonPressed = true;
                }
            }
            else if (buttons_read() & BUTTONS_BTN2_MASK) {
                if (!buttonPressed) {
                    tetrisDisplay_rotateShape(&currentShape, board);
                    buttonPressed = true;
                }
            }
            else {
                buttonPressed = false;
            }
            fallCounter++;
            break;
        case check_row_st: {
                uint8_t bonusMultiplier = 1;
                for (uint8_t i = NUM_ROWS - 1; i > 0;) {
                    i--;
                    for (uint8_t j = 0; j < NUM_COLS; j++) {
                        if (board[j][i].filled) {
                            if (j == NUM_COLS - 2) {
                                tetrisControl_inGameScore(true);
                                currentScore += 10 * bonusMultiplier++;
                                tetrisControl_inGameScore(false);
                                tetrisDisplay_eraseFullLine(i, board);
                                tetrisDisplay_moveLinesDown(i++, board);
                            }
                            else {
                                continue;
                            }
                        }
                        else {
                            break;
                        }
                    }
                }
            }
            break;
        case lose_st:
            break;
        case disp_score_st:
            scoreTime++;
            break;
        case lower_switch_st:
            break;
    }
}
