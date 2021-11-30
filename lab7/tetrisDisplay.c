#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "utils.h"
#include "tetrisDisplay.h"

#define LEFT_SIDE   (DISPLAY_WIDTH / 2 - DRAW_SIZE * NUM_COLS / 2)
#define TOP_SIDE   (DISPLAY_HEIGHT / 2 - DRAW_SIZE * NUM_ROWS / 2)

#define START_X   5
#define START_Y   0

#define SPACING   1

Box board[NUM_COLS][NUM_ROWS];
Box placed[MAX_PLACEABLE];

uint8_t numPlaced;

void tetrisDisplay_init() {
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

bool tetrisDisplay_sideCollision(Shape* activeShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        if (activeShape->boxes[i].x_pos - 1 == 0 || activeShape->boxes[i].x_pos + 1 == NUM_COLS - 1) {
            return true;
        }
    }
    return false;
}

bool tetrisDisplay_bottomCollision(Shape* activeShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        if (activeShape->boxes[i].y_pos + 1 == NUM_ROWS - 1) {
            return true;
        }
    }
    return false;
}

void tetrisDisplay_setBoxColor(Shape* activeShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        activeShape->boxes[i].color = activeShape->color;
        board[activeShape->boxes[i].x_pos][activeShape->boxes[i].y_pos].color = activeShape->color;
    }
}

void tetrisDisplay_makeShape(Shape* activeShape, uint8_t shapeNum) {
    if (shapeNum == rndm) {
        // DON'T KEEP THIS
        int MAGIC_NUMBER_SEED = 547;
        srand(MAGIC_NUMBER_SEED);
        shapeNum = rand() % 7;
    }

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
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum] = board[START_X][START_Y + 1];
            break;
        case t:
            activeShape->color = T_COLOR;
            activeShape->boxes[boxNum++] = board[START_X - 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum] = board[START_X][START_Y + 1];
            break;
        case l:
            activeShape->color = L_COLOR;
            activeShape->boxes[boxNum++] = board[START_X][START_Y + 1];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y];
            break;
        case z:
            activeShape->color = Z_COLOR;
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y + 1];
            break;
        case l_inv:
            activeShape->color = L_INV_COLOR;
            activeShape->boxes[boxNum++] = board[START_X - 1][START_Y];
            activeShape->boxes[boxNum++] = board[START_X][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            break;
        case z_inv:
            activeShape->color = Z_INV_COLOR;
            activeShape->boxes[boxNum++] = board[START_X + 2][START_Y];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y];
            activeShape->centerBox = &activeShape->boxes[boxNum];
            activeShape->boxes[boxNum++] = board[START_X + 1][START_Y + 1];
            activeShape->boxes[boxNum++] = board[START_X][START_Y + 1];
            break;
    }

    tetrisDisplay_setBoxColor(activeShape);
}

void tetrisDisplay_drawShape(Shape* activeShape) {
    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        tetrisDisplay_drawBox(&activeShape->boxes[i]);
    }
}

void tetrisDisplay_moveShape(Shape* activeShape, bool right) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser);

    if (right && !tetrisDisplay_sideCollision(activeShape)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].x_pos++;
        }
    }
    else if (!right && !tetrisDisplay_sideCollision(activeShape)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].x_pos--;
        }
    }
    else return;
    tetrisDisplay_drawShape(&eraser);    
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape);
}

void tetrisDisplay_rotateShape(Shape* activeShape) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser);

    for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
        uint8_t x_offset = activeShape->boxes[i].x_pos - activeShape->centerBox->x_pos;
        uint8_t y_offset = activeShape->boxes[i].y_pos - activeShape->centerBox->y_pos;
        activeShape->boxes[i].x_pos = activeShape->centerBox->x_pos - y_offset;
        activeShape->boxes[i].y_pos = activeShape->centerBox->y_pos + x_offset;
    }

    tetrisDisplay_drawShape(&eraser);
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape);
}

void tetrisDisplay_drawBox(Box* activeBox) {
    uint16_t x = LEFT_SIDE + activeBox->x_pos * DRAW_SIZE;
    uint16_t y = TOP_SIDE + activeBox->y_pos * DRAW_SIZE;
    display_fillRect(x + SPACING, y + SPACING, DRAW_SIZE - SPACING - SPACING, DRAW_SIZE - SPACING - SPACING, activeBox->color);
}

void tetrisDisplay_fall(Shape* activeShape) {
    Shape eraser = *activeShape;
    eraser.color = DISPLAY_BLACK;
    tetrisDisplay_setBoxColor(&eraser);
    if (!tetrisDisplay_bottomCollision(activeShape)) {
        for (uint8_t i = 0; i < SHAPE_SIZE; i++) {
            activeShape->boxes[i].y_pos++;
        }
    }
    else return;
    tetrisDisplay_drawShape(&eraser);
    tetrisDisplay_drawShape(activeShape);
    tetrisDisplay_setBoxColor(activeShape);
}

bool tetrisDisplay_findCollision(Shape* activeShape) {

}

void tetrisDisplay_test() {
    display_init();
    tetrisDisplay_init();
    Shape currentShape;
    tetrisDisplay_makeShape(&currentShape, 6);
    tetrisDisplay_drawShape(&currentShape);
    utils_msDelay(100);
    tetrisDisplay_fall(&currentShape);
    utils_msDelay(100);
    tetrisDisplay_fall(&currentShape);
    utils_msDelay(100);
    tetrisDisplay_fall(&currentShape);
    utils_msDelay(1000);
    tetrisDisplay_rotateShape(&currentShape);
    utils_msDelay(1000);
    tetrisDisplay_rotateShape(&currentShape);
    utils_msDelay(1000);
    tetrisDisplay_rotateShape(&currentShape);
    utils_msDelay(1000);
    tetrisDisplay_rotateShape(&currentShape);
}
