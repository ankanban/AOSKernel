
#include <task.h>
#include <vmm.h>
#include <x86/cr.h>
#include <x86/eflags.h>


void
task_switch(task_t * task)
{
  /* Create page directory and tables */
  vmm_init(task);
  vmm_create_stack(&task->thread);
 

  /* Save kernel stack pointer */
  set_esp0();
 
  
  /* Set Control registers */
  uint32_t cr0 = get_cr0();
  cr0 |= CR0_PG;
  //unsigned int cr3 = get_cr3();
  set_cr3(task->page_dir);
  uint32_t cr4  = get_cr4();
  cr4 |= CR4_PGE;
  set_cr4(cr4);

  /* set eflags */
  uint32_t eflags = get_eflags();
  eflags |= EFL_CF | EFL_IOPL_RING0 | EFL_IF;
  eflags &= ~EFL_AC;

  task->flags = eflags;

  //set_eflags(eflags);
  
  /* Now call a wrapper that can build the stack
   * and call IRET to transit into user-mode.
   */

  /* Currently hard-coded to switch to task_idle */
  do_task_switch(task_idle, 
		 eflags, 
		 cs, 
		 ss, 
		 ds);
}
