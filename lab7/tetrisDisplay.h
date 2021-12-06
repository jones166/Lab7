#ifndef TETRISDISPLAY_H
#define TETRISDISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "display.h"

#define DRAW_SIZE   11

#define NUM_COLS   12
#define NUM_ROWS   22

#define LINE_COLOR     DISPLAY_CYAN
#define T_COLOR        DISPLAY_MAGENTA
#define SQUARE_COLOR   DISPLAY_YELLOW
#define L_COLOR        0xFD00
#define L_INV_COLOR    DISPLAY_BLUE
#define Z_COLOR        DISPLAY_RED
#define Z_INV_COLOR    DISPLAY_GREEN
#define EDGE_COLOR     DISPLAY_GRAY

#define SHAPE_SIZE   4
#define MAX_PLACEABLE   200

#define LEFT_SIDE   (DISPLAY_WIDTH / 2 - DRAW_SIZE * NUM_COLS / 2)
#define TOP_SIDE   (DISPLAY_HEIGHT / 2 - DRAW_SIZE * NUM_ROWS / 2)

#define START_X   5
#define START_Y   0

enum shape_value_t { line, square, t, l, z, l_inv, z_inv };

typedef struct box{
  uint16_t color;
  int8_t x_pos;
  int8_t y_pos;
  bool filled;
} Box;

typedef struct shape{
  uint16_t color;
  Box* centerBox;
  Box boxes[SHAPE_SIZE];
} Shape;

void tetrisDisplay_init(Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_makeShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS], uint8_t shapeNum);

void tetrisDisplay_drawShape(Shape* activeShape);

void tetrisDisplay_getNextShape(Shape* nextShape, Box board[NUM_COLS][NUM_ROWS], uint16_t seed);

void tetrisDisplay_drawNextShape(Shape* nextShape);

void tetrisDisplay_eraseNextShape(Shape* nextShape);

void tetrisDisplay_updateCurrent(Shape* nextShape, Shape* currentShape, Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_moveShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS], bool right);

void tetrisDisplay_rotateShape(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_drawBox(Box* activeBox);

void tetrisDisplay_fall(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

bool tetrisDisplay_bottomCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

bool tetrisDisplay_rotateCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

bool tetrisDisplay_leftCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

bool tetrisDisplay_rightCollision(Shape* activeShape, Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_eraseFullLine(uint8_t yCoord, Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_moveLinesDown(uint8_t yCoord, Box board[NUM_COLS][NUM_ROWS]);

void tetrisDisplay_test();

#endif /* TETRISDISPLAY_H */
