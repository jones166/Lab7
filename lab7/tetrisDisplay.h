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

void tetrisDisplay_init();

bool tetrisDisplay_leftCollision(Shape* activeShape);

bool tetrisDisplay_rightCollision(Shape* activeShape);

bool tetrisDisplay_bottomCollision(Shape* activeShape);

bool tetrisDisplay_rotateCollision(Shape* activeShape);

void tetrisDisplay_makeShape(Shape* activeShape, uint8_t shapeNum);

void tetrisDisplay_getNextShape(Shape* newShape, uint16_t seed);

void tetrisDisplay_drawShape(Shape* activeShape);

void tetrisDisplay_drawNextShape(Shape* nextShape, bool erase);

void tetrisDisplay_moveShape(Shape* activeShape, bool right);

void tetrisDisplay_rotateShape(Shape* activeShape);

void tetrisDisplay_drawBox(Box* activeBox);

void tetrisDisplay_fall(Shape* activeShape);

bool tetrisDisplay_findCollision(Shape* activeShape);

void tetrisDisplay_eraseFullLine(uint8_t yCoord);

void tetrisDisplay_moveLinesDown(uint8_t yCoord);

void tetrisDisplay_test();

#endif /* TETRISDISPLAY_H */
