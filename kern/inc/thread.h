#ifndef __THREAD_H__
#define __THREAD_H__

#include <vm_area.h>
#include <stdint.h>
struct task;

#define THREAD_DEFAULT_STACK_SIZE (16 << PAGE_SHIFT)

typedef struct {
  unsigned int id;

  /* Stack state */
  vm_area_t vm_stack;
  uint32_t kernel_esp;
  uint32_t esp;
  uint32_t eip;
  uint32_t ss;
  uint32_t eflags;

  void * entry_point;

  struct task * task;
  /* List ptr for thread list */
} thread_t;

void 
thread_create(thread_t * thread,
	      struct task * task,
	      unsigned int user_stack_size,
	      unsigned int kernel_stack_size);

void
thread_switch(thread_t * thread);

void
thread_save_state(uint32_t eip, 
		  uint32_t esp);

#endif
