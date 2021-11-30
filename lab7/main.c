#include <stdio.h>
#include "display.h"
#include "utils.h"
#include "interrupts.h"
#include "tetrisDisplay.h"

int main() {
  tetrisDisplay_test();
}

void isr_function() {
  // Empty for now.
}
