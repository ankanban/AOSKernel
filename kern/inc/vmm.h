#ifndef __VMM_H__
#define __VMM_H__

/* x86 specific includes */
#include <x86/seg.h>                /* install_user_segs() */
#include <x86/interrupt_defines.h>  /* interrupt_setup() */
#include <x86/asm.h>                /* enable_interrupts() */
#include <x86/page.h>
#include <vmm_page.h>
#include <vm_area.h>
#include <task.h>


#define THREAD_STACK_MARGIN 63

void
vmm_kernel_init();

void
vmm_task_init(task_t * task);

void
vmm_thread_init(thread_t * thread,
		unsigned int user_stack_size,
		unsigned int kernel_stack_size);


#endif /* __VMM_H__ */
