/**
 * @file timer.c
 * @brief Timer driver implementation
 * @author Anshuman P.Kanetkar
 */
//#include <p1kern.h>
#include <simics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86/asm.h>
#include <x86/seg.h>
#include <x86/interrupt_defines.h>
#include <x86/timer_defines.h>
#include <x86/keyhelp.h>
#include <x86/video_defines.h>
#include <interrupt_wrappers.h>
#include <handler_install.h>
#include <timer.h>

// timer driver data

static unsigned int numTicks;
static void (*timer_callback)(unsigned int);

void
timer_init(void (*tickback)(unsigned int))
{
  timer_callback = tickback;
  
  //Set timer handler
  set_idt_entry(TIMER_IDT_ENTRY,
		timer_wrapper);
  
  // Set the timer mode
  outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
  // Set the timer interrupt frequency
  outb(TIMER_PERIOD_IO_PORT, lsb(TIMER_CYCLES_PER_INT));
  outb(TIMER_PERIOD_IO_PORT, msb(TIMER_CYCLES_PER_INT));

}

/*
 * 
 */
void 
timer_interrupt_handler(void)
{
  numTicks++;
  timer_callback(numTicks);
  outb(INT_CTL_PORT, INT_ACK_CURRENT);
}
