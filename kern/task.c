
#include <stdint.h>
#include <task.h>
#include <vmm.h>
#include <x86/cr.h>
#include <x86/eflags.h>
#include <x86/seg.h>
//#include <task_idle.h>
#include <simics.h>
#include <elf_410.h>
#include <sched.h>

int current_task_id = 0;
int current_thread_id = 0;

uint32_t kernel_saved_stack_ptr;


void
task_idle();

void
print_elf_header(simple_elf_t * se_hdr)
{
  lprintf("Name         : %s", se_hdr->e_fname);
  lprintf("Entry        : %p", se_hdr->e_entry);
  lprintf("txtoff       : %p", se_hdr->e_txtoff);
  lprintf("e_txtlen     : %u", se_hdr->e_txtlen);
  lprintf("e_txtstart   : %p", se_hdr->e_txtstart);    /* start of text segment virtual address */
  lprintf("e_datoff     : %p", se_hdr->e_datoff);      /* offset of data segment in file */
  lprintf("e_datlen     : %p", se_hdr->e_datlen);      /* length of data segment in bytes */
  lprintf("e_datstart   : %p", se_hdr->e_datstart);    /* start of data segment in virtual memory */
  lprintf("e_rodatoff   : %p", se_hdr->e_rodatoff);    /* offset of rodata segment in file */
  lprintf("e_rodatlen   : %u", se_hdr->e_rodatlen);    /* length of rodata segment in bytes */
  lprintf("e_rodatstart : %p", se_hdr->e_rodatstart);  /* start of rodata segment in virtual memory*/
  lprintf("e_bsslen     : %u", se_hdr->e_bsslen);      /* length of bss  segment in bytes */

}


void
task_load_text(task_t * task,
	       simple_elf_t * se_hdr)
{ 
  int bytes_read = 0;
  vm_area_t vma;

  vm_area_create(&vma,
		 (void *)se_hdr->e_txtstart,
		 se_hdr->e_txtlen,
		 PGTAB_ATTRIB_USR_RO,
		 task);
  
  bytes_read = vm_area_load(&vma,
			    se_hdr->e_fname,
			    se_hdr->e_txtoff,
			    se_hdr->e_txtlen,
			    (void *)se_hdr->e_txtstart);

  lprintf("Loaded %d bytes text seg @ %p\n", 
	  bytes_read,
	  se_hdr->e_txtstart);

}


void
task_load_rodata(task_t * task,
		 simple_elf_t * se_hdr)
{
  int bytes_read = 0;
  vm_area_t vma;

  vm_area_create(&vma,
		 (void *)se_hdr->e_rodatstart,
		 se_hdr->e_rodatlen,
		 PGTAB_ATTRIB_USR_RO,
		 task);
  
  bytes_read = vm_area_load(&vma,
			    se_hdr->e_fname,
			    se_hdr->e_rodatoff,
			    se_hdr->e_rodatlen,
			    (void *)se_hdr->e_rodatstart);

  lprintf("Loaded %d bytes rodata seg @ %p\n", 
	  bytes_read,
	  se_hdr->e_rodatstart);

}


void
task_load_data(task_t * task,
	       simple_elf_t * se_hdr)
{
  int bytes_read = 0;
  vm_area_t vma;

  vm_area_create(&vma,
		 (void *)se_hdr->e_datstart,
		 se_hdr->e_datlen + se_hdr->e_bsslen,
		 PGTAB_ATTRIB_USR_RW,
		 task);
  
  bytes_read = vm_area_load(&vma,
			    se_hdr->e_fname,
			    se_hdr->e_datoff,
			    se_hdr->e_datlen,
			    (void *)se_hdr->e_datstart);
  
  lprintf("Loaded %d bytes data seg @ %p\n", 
	  bytes_read,
	  se_hdr->e_datstart);
  
  vm_area_zero(&vma,
	       (void *)(se_hdr->e_datstart + se_hdr->e_datlen),
	       se_hdr->e_bsslen);

  lprintf("Initialized %d bytes bss seg @ %p\n", 
	  se_hdr->e_bsslen,
	  se_hdr->e_datstart + se_hdr->e_datlen);
  
}



void
task_load(task_t * task,
	  thread_t * thread,
	  char * task_path)
{


  /* Use getbytes to load the executable's pages */
  simple_elf_t se_hdr;
  elf_load_helper(&se_hdr, task_path);
  print_elf_header(&se_hdr);

  task_load_text(task, &se_hdr);

  task_load_rodata(task, &se_hdr);
  
  task_load_data(task, &se_hdr);

  // Set the entry point for the thread.
  thread->eip = se_hdr.e_entry;

}


void 
thread_create(thread_t * thread,
	      task_t * task,
	      unsigned int user_stack_size,
	      unsigned int kernel_stack_size)
{
  thread->id = ++current_thread_id;
  thread->task = task;

  thread->esp = ~0UL;
  thread->eip = 0;
  thread->ss = SEGSEL_USER_DS;
  thread->eflags = 0;
    /* set eflags */
  uint32_t eflags = get_eflags();
  eflags = EFL_CF | EFL_IOPL_RING0 | EFL_IF;
  eflags &= ~EFL_AC;
  
  thread->eflags = eflags;

  vmm_thread_init(thread, 
		  user_stack_size,
		  kernel_stack_size);

  lprintf("Created stack");  
}

void
task_create(task_t * task)
{

  task->id = ++current_task_id;
  /* Create page directory and tables */
  vmm_task_init(task);
}

void
thread_switch(thread_t * thread)
{
  
  lprintf("Set sp0");

  //MAGIC_BREAK;

  /* Set Control registers */

  //unsigned int cr3 = get_cr3();
  set_cr3((unsigned int)thread->task->page_dir);

  uint32_t cr4  = get_cr4();
  cr4 |= CR4_PGE;
  set_cr4(cr4);

  lprintf("Set CRs 3 & 4");
  
  /* Now call a wrapper that can build the stack
   * and call IRET to transit into user-mode.
   */
  set_esp0(thread->kernel_esp);

  lprintf("Kernel ESP0 Saved to: 0x%x", thread->kernel_esp); 

  lprintf("eip: 0x%x, cs: 0x%x, efl: 0x%x, esp: 0x%x, kesp: 0x%x, ss: 0x%x, ds: 0x%x",
	  thread->eip,
	  thread->task->cs,
	  thread->eflags,
	  thread->esp,
	  thread->kernel_esp,
	  thread->ss,
	  thread->task->ds);

  lprintf("about to switch task");

  sched_set_current_thread(thread);
  
  //MAGIC_BREAK;

  /* Currently hard-coded to switch to task_idle */
  do_task_switch(thread->eip, 
		 thread->task->cs,
		 thread->eflags, 
		 thread->esp,
		 thread->ss,
		 thread->task->ds);
}

