#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "utils.h"
#include "buttons.h"
#include "tetrisDisplay.h"

#define SPACING   1

// Box board[NUM_COLS][NUM_ROWS];
Box placed[MAX_PLACEABLE];

uint8_t numPlaced;

void tetrisDisplay_init(Box board[NUM_COLS][NUM_ROWS]) {
    numPlaced = 0;
    display_fillScreen(DISPLAY_BLACK);
    for (uint8_t i = 0; i < NUM_COLS; i++) {
        for (uint8_t j = 0; j < NUM_ROWS; j++) {
            board[i][j].x_pos = i;
            board[i][j].y_pos = j;
            if ((i == 0 || i == NUM_COLS - 1) || (j == NUM_ROWS - 1)) {
                board[i][j].color = EDGE_COLOR;
                board[i][j].filled = true;
                tetrisDisplay_drawBox(&board[i][j]);
            }
            else {
                board[i][j].color = DISPLAY_BLACK;
                board[i][j].filled = false;
            }
        }
    }
}

bool tetrisDisplay_leftCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        if (board[activeShape->boxes[i].x_pos - 1][activeShape->boxes[i].y_pos].filled) {
            return true;
        }
    }
    return false;
}

bool tetrisDisplay_rightCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        if (board[activeShape->boxes[i].x_pos + 1][activeShape->boxes[i].y_pos].filled) {
            return true;
        }
    }
    return false;
}

bool tetrisDisplay_bottomCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        if (board[activeShape->boxes[i].x_pos][activeShape->boxes[i].y_pos + 1].filled) {
            return true;
        }
    }
    return false;
}

bool tetrisDisplay_rotateCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        int8_t x_offset = activeShape->boxes[i].x_pos - activeShape->centerBox->x_pos;
        int8_t y_offset = activeShape->boxes[i].y_pos - activeShape->centerBox->y_pos;
        if (board[activeShape->boxes[i].x_pos - y_offset][activeShape->boxes[i].y_pos + x_offset].filled) {
            return true;
        }
    }
    return false;
}

void tetrisDisplay_setBoxColor(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        activeShape->boxes[i].color = activeShape->color;
        board[activeShape->boxes[i].x_pos][activeShape->boxes[i].y_pos].color = activeShape->color;
    }
}

void tetrisDisplay_getNextShape(Shape* newShape, Box board[NUM_COLS][NUM_ROWS], uint16_t seed) {
    srand(seed);
    uint8_t shapeNum = rand() % 6;
    tetrisDisplay_makeShape(newShape, board, shapeNum);
}

void tetrisDisplay_makeShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS], uint8_t shapeNum) {
    uint8_t boxNum = 0;
    switch (shapeNum) {
        case line:
            activeShape->color = LINE_COLOR;
            activeShape->boxes[boxNum++] = board[START_X - 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum] = board[START_X + 2][START_Y];
            break;
        case square:
            activeShape->color = SQUARE_COLOR;
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum] = board[START_X][START_Y + 1];
            break;
        case t:
            activeShape->color = T_COLOR;
            activeShape->boxes[boxNum++] = board[START_X - 1][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum] = board[START_X][START_Y + 1];
            break;
        case l:
            activeShape->color = L_COLOR;
            activeShape->boxes[boxNum++] = board[START_X][START_Y + 1];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y];
            break;
        case z:
            activeShape->color = Z_COLOR;
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y + 1];
            break;
        case l_inv:
            activeShape->color = L_INV_COLOR;
            activeShape->boxes[boxNum++] = board[START_X - 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            break;
        case z_inv:
            activeShape->color = Z_INV_COLOR;
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum++] = board[START_X][START_Y + 1];
            break;
    }
    
    tetrisDisplay_setBoxColor(activeShape, board);
}

void tetrisDisplay_drawShape(Shape* activeShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        tetrisDisplay_drawBox(&activeShape->boxes[i]);
    }
}

void tetrisDisplay_drawNextShape(Shape* nextShape) {
    display_setTextSize(1);
    display_setCursor(1, DISPLAY_HEIGHT/2);
    display_setTextColor(DISPLAY_WHITE);
    display_println("Next Shape");

    int8_t dist_x[SHAPE_SIZE];
    int8_t dist_y[SHAPE_SIZE];
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        dist_x[i] = nextShape->centerBox->x_pos - nextShape->boxes[i].x_pos;
        dist_y[i] = nextShape->centerBox->y_pos - nextShape->boxes[i].y_pos;
    }
    nextShape->centerBox->x_pos = -6;
    nextShape->centerBox->y_pos = 13;
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        nextShape->boxes[i].x_pos = nextShape->centerBox->x_pos + dist_x[i];
        nextShape->boxes[i].y_pos = nextShape->centerBox->y_pos + dist_y[i];
        tetrisDisplay_drawBox(&nextShape->boxes[i]);
    }
}

void tetrisDisplay_eraseNextShape(Shape* nextShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        nextShape->boxes[i].color = DISPLAY_BLACK;
        tetrisDisplay_drawBox(&nextShape->boxes[i]);
    }
}

void tetrisDisplay_updateCurrent(Shape* nextShape, Shape* currentShape, Box board[NUM_COLS][NUM_ROWS]) {
    switch (nextShape->color) {
        case LINE_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 0);
            break;
        case SQUARE_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 1);
            break;
        case T_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 2);
            break;
        case L_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 3);
            break;
        case Z_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 4);
            break;
        case L_INV_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 5);
            break;
        case Z_INV_COLOR:
            tetrisDisplay_makeShape(currentShape, board, 6);
            break;
    }
}

void tetrisDisplay_moveShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS], bool right) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser, board);

    if (right && !tetrisDisplay_rightCollision(activeShape, board)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].x_pos++;
        }
    }
    else if (!right && !tetrisDisplay_leftCollision(activeShape, board)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].x_pos--;
        }
    }
    else return;
    tetrisDisplay_drawShape(&eraser);    
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape, board);
}

void tetrisDisplay_rotateShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser, board);

    if (activeShape->color == SQUARE_COLOR) return;
    else if (!tetrisDisplay_rotateCollision(activeShape, board)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            int8_t x_offset = activeShape->boxes[i].x_pos - activeShape->centerBox->x_pos;
            int8_t y_offset = activeShape->boxes[i].y_pos - activeShape->centerBox->y_pos;
            activeShape->boxes[i].x_pos = activeShape->centerBox->x_pos - y_offset;
            activeShape->boxes[i].y_pos = activeShape->centerBox->y_pos + x_offset;
        }
    }
    else return;

    tetrisDisplay_drawShape(&eraser);
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape, board);
}

void tetrisDisplay_drawBox(Box* activeBox) {
    uint16_t x = LEFT_SIDE + activeBox->x_pos * DRAW_SIZE;
    uint16_t y = TOP_SIDE + activeBox->y_pos * DRAW_SIZE;
    display_fillRect(x + SPACING, y + SPACING, DRAW_SIZE - SPACING - SPACING, DRAW_SIZE - SPACING - SPACING, activeBox->color);
}

void tetrisDisplay_fall(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser, board);
    if (!tetrisDisplay_bottomCollision(activeShape, board)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].y_pos++;
        }
    }
    else return;
    tetrisDisplay_drawShape(&eraser);
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape, board);
}

void tetrisDisplay_eraseFullLine(uint8_t yCoord, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 1; i < NUM_COLS - 1; i++) {
        board[i][yCoord].filled = false;
        board[i][yCoord].color = DISPLAY_BLACK;
        tetrisDisplay_drawBox(&board[i][yCoord]);
    }
}

void tetrisDisplay_moveLinesDown(uint8_t yCoord, Box board[NUM_COLS][NUM_ROWS]) {
    for (uint8_t i = 1; i < NUM_COLS - 1; i++) {
        for (uint8_t j = yCoord; j != 0;) {
            j--;
            if (board[i][j].filled) {
                board[i][j+1].filled = true;
                board[i][j+1].color = board[i][j].color;
                tetrisDisplay_drawBox(&board[i][j+1]);
                board[i][j].filled = false;
                board[i][j].color = DISPLAY_BLACK;
                tetrisDisplay_drawBox(&board[i][j]);
            }
        }
    }
}

void tetrisDisplay_test() {
    display_init();
    buttons_init();
    Box board[NUM_COLS][NUM_ROWS];
    tetrisDisplay_init(board);
    Shape currentShape;
    tetrisDisplay_getNextShape(&currentShape, board, 2);
    tetrisDisplay_drawShape(&currentShape);
    Shape nextShape;
    tetrisDisplay_getNextShape(&nextShape, board, 800);
    tetrisDisplay_drawNextShape(&nextShape);
    uint64_t timer = 0;
    uint64_t counter = 0;
    printf("Starting loop.\n");
    while (!(buttons_read() & BUTTONS_BTN3_MASK)) {
        if (++timer == 5000000) {
            if (tetrisDisplay_bottomCollision(&currentShape, board)) {
                for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
                    board[currentShape.boxes[i].x_pos][currentShape.boxes[i].y_pos] = currentShape.boxes[i];
                    board[currentShape.boxes[i].x_pos][currentShape.boxes[i].y_pos].filled = true;
                }
                tetrisDisplay_updateCurrent(&nextShape, &currentShape, board);
                tetrisDisplay_eraseNextShape(&nextShape);
                tetrisDisplay_getNextShape(&nextShape, board, timer + counter);
                tetrisDisplay_drawNextShape(&nextShape);

                tetrisDisplay_drawShape(&currentShape);
            }
            timer = 0;
            tetrisDisplay_fall(&currentShape, board);
        }
        if (buttons_read() & BUTTONS_BTN0_MASK) {
            tetrisDisplay_moveShape(&currentShape, board, true);
            counter++;
            while(buttons_read() & BUTTONS_BTN0_MASK);
        }
        if (buttons_read() & BUTTONS_BTN1_MASK) {
            tetrisDisplay_moveShape(&currentShape, board, false);
            counter++;
            while(buttons_read() & BUTTONS_BTN1_MASK);
        }
        if (buttons_read() & BUTTONS_BTN2_MASK) {
            tetrisDisplay_rotateShape(&currentShape, board);
            counter++;
            while(buttons_read() & BUTTONS_BTN2_MASK);
        }
    }
}
