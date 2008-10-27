/** @file handler_install.c 
 *
 *  @brief Driver implementation.
 *
 *  @author Anshuman P.Kanetkar (apk)
 * 
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
#include <inc/interrupt_wrappers.h>
#include <inc/handler_install.h>
#include <inc/timer.h>
#include <inc/keyboard.h>
#include <inc/console.h>


/**
 * @brief Set the idt entry at the given index
 */
void 
set_idt_entry(int index, 
	      void (*handler)(void))
{
  idt_gate_desc_t * idt_entry = (idt_gate_desc_t *)idt_base();
  unsigned short *handler_offset = (unsigned short *)&handler;

  //lprintf("idt_base: %p\n", idt_entry);

  idt_entry += index;

  idt_entry->unused = 0;
  idt_entry->offset_low = handler_offset[0];
  idt_entry->offset_high = handler_offset[1];
  idt_entry->flags = IDT_FLAGS_TRAP_GATE ;
  idt_entry->seg_sel = SEGSEL_KERNEL_CS;
  /*
   *lprintf("off_lo:0x%04x, off_hi:0x%04x, segsel:0x%04x, flags:0x%02x\n",
   *	  idt_entry->offset_low,
   *  idt_entry->offset_high,
   *  idt_entry->seg_sel,
   *  idt_entry->flags);
   */
}

/**
 * @brief Installs the timer tick handler, and initializes
 * timer, keyboard and console drivers.
 */
int 
handler_install(void (*tickback)(unsigned int))
{
  
  //  lprintf("Timer handler :%p\n", timer_wrapper);
  //lprintf("Keyboard handler :%p\n", keyboard_wrapper);

  timer_init(tickback);
 
  keyboard_init();

  console_init();
  
  //test_console();

  return 0;
}


