/** @file kernel.c
 *  @brief An initial kernel.c
 *
 *  You should initialize things in kernel_main(),
 *  and then run stuff.
 *
 *  @author Harry Q. Bovik (hqbovik)
 *  @author Fred Hacker (fhacker)
 *  @bug No known bugs.
 */

#include <common_kern.h>

/* libc includes. */
#include <stdio.h>
#include <simics.h>                 /* lprintf() */
#include <malloc.h>
#include <stddef.h>

/* multiboot header file */
#include <multiboot.h>              /* boot_info */

/* memory includes. */
#include <lmm.h>                    /* lmm_remove_free() */

/* x86 specific includes */
#include <x86/seg.h>                /* install_user_segs() */
#include <x86/interrupt_defines.h>  /* interrupt_setup() */
#include <x86/asm.h>                /* enable_interrupts() */
#include <interrupt_wrappers.h>
#include <kernel_timer.h>
#include <console.h>
#include <keyboard.h>
#include <timer.h>
#include <handler_install.h>
#include <task.h>
#include <thread.h>
#include <vmm.h>
#include <syscall_int.h>
#include <kernel_asm.h>
#include <sched.h>

/*
 * state for kernel memory allocation.
 */
extern lmm_t malloc_lmm;

/*
 * Info about system gathered by the boot loader
 */
extern struct multiboot_info boot_info;

task_t idle_task;
int default_stack_size = (16 << PAGE_SHIFT);
char * idle_task_path = "ck1";

int
sys_gettid(void)
{
  int thread_id = 0;
  thread_t * current = NULL;

  //MAGIC_BREAK;
  //set_kernel_ds();
  current = sched_get_current_thread();
  thread_id = current->id;
  //set_user_ds();

  return thread_id;
}


void gettid_wrapper(void);

void
syscall_init()
{
  lprintf("Setting up system call vectors");
  set_syscall_entry(GETTID_INT, gettid_wrapper);
  
}

/** @brief Kernel entrypoint.
 *  
 *  This is the entrypoint for the kernel.
 *
 * @return Does not return
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp)
{
    /*
     * Tell the kernel memory allocator which memory it can't use.
     * It already knows not to touch kernel image.
     */

    /* Everything above 16M */
    lmm_remove_free( &malloc_lmm, (void*)USER_MEM_START, -8 - USER_MEM_START );
    
    /* Everything below 1M  */
    lmm_remove_free( &malloc_lmm, (void*)0, 0x100000 );


    /*
     * Install the timer handler function.
     */
    handler_install(kernel_tick);
    syscall_init();

    /*
     * initialize the PIC so that IRQs and
     * exception handlers don't overlap in the IDT.
     */
    interrupt_setup();

    /*
     * When kernel_main() begins, interrupts are DISABLED.
     * You should delete this comment, and enable them --
     * when you are ready.
     */

    lprintf( "Hello from a brand new kernel!" );

    enable_interrupts();

    //MAGIC_BREAK;

    vmm_kernel_init();

    //while (1) {
    //    continue;
    //}

    task_create(&idle_task);

    thread_create(&idle_task.thread,
		  &idle_task,
		  default_stack_size,
		  default_stack_size);
    
    /* Load the task */
    task_load(&idle_task,
	      &idle_task.thread,
	      idle_task_path);

    sched_set_current_thread(&idle_task.thread);

    sched_yield();

    return 0;
}
