
#include <stdio.h>
#include "display.h"
#include "config.h"
#include "utils.h"
#include "interrupts.h"
#include "tetrisDisplay.h"
#include "tetrisControl.h"
#include "interrupts.h"
#include "xparameters.h"

#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((CONFIG_TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)

#define INTERRUPTS_PER_SECOND (1.0 / CONFIG_TIMER_PERIOD)
#define TOTAL_SECONDS 20
#define MAX_INTERRUPT_COUNT (INTERRUPTS_PER_SECOND * TOTAL_SECONDS)

// Keep track of how many times isr_function() is called.
uint32_t isr_functionCallCount = 0;


int main() {        
  interrupts_initAll(true);
  interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
  interrupts_enableTimerGlobalInts();
  tetrisControl_init();
  // Start the private ARM timer running.
  interrupts_startArmPrivateTimer();
  // Enable interrupts at the ARM.
  interrupts_enableArmInts();
  while (1) {
    if (interrupts_isrFlagGlobal) {
      tetrisControl_tick();
      interrupts_isrFlagGlobal = 0;
      utils_sleep();
      fflush(stdout);
    }
  }
  interrupts_disableArmInts();
  return 0;
}

void isr_function() {
  // Empty for now.
}
