#include <stddef.h>
#include <string.h>
#include <x86/page.h>
#include <x86/cr.h>
#include <vmm_page.h>
#include <vmm.h>
#include <common_kern.h>
#include <simics.h>
#include <stdint.h>

void
vmm_kernel_init()
{
  vmm_page_init();
  return;
}


/* 
 * Create the basic virtual memory map for the task
 */
void
vmm_task_init(task_t * task)
{
  unsigned int cr3 = get_cr3();
  pde_t * kpbdr = (pde_t *)(cr3 & PAGE_MASK);
  pde_t * upbdr = NULL ;

  task->cs = SEGSEL_USER_CS;
  task->ds = SEGSEL_USER_DS;

  upbdr = vmm_create_tables(PGDIR_SIZE);
  task->page_dir = upbdr;
  
  lprintf("Kernel Pgdir base: %p", kpbdr);
  lprintf("User Pgdir base: %p", upbdr);

  /* Map the kernel area */
  vm_area_init(&task->vma_kernel);
  
  vm_area_create_identity(&task->vma_kernel,
			  0,
			  USER_MEM_START,
			  PGTAB_ATTRIB_SU_RW,
			  task);
  
}

/*
 * Create the kernel stack for the thread
 */
void
vmm_kernel_stack_init(thread_t * thread,
		      unsigned int stack_size)
{
  pde_t * page_dir = (pde_t *)thread->task->page_dir;

  unsigned int num_pages = ((stack_size - 1) >> PAGE_SHIFT) + 1;
  void * kernel_stack = NULL;

  lprintf("Allocating %u bytes for kernel stack (%u pages)", 
	  stack_size,
	  num_pages);

  //MAGIC_BREAK;

  // Allocate stack from high memory
  kernel_stack = vmm_ppfs_highmem_alloc(num_pages);

  lprintf("kernel stack for thread %d: %p", kernel_stack, thread->id);
  
  vmm_map_pages(page_dir,
		(void *)(kernel_stack - (num_pages << PAGE_SHIFT) + 1),
		(void *)(kernel_stack - (num_pages << PAGE_SHIFT) + 1),
		num_pages,
		PGTAB_ATTRIB_USR_RW);
  
  thread->kernel_esp = (uint32_t)kernel_stack - THREAD_STACK_MARGIN;
  

  // Set the thread's esp to just below the kernel stack (separated
  // by a buffer null page)
  thread->esp = ((uint32_t)kernel_stack - ((num_pages + 1) 
					   << PAGE_SHIFT));
  
}


/* Create a Stack at the very top of the
 * address space. XXX Should allocate a stack by 
 * dynamically allocating pages, and mapping it into
 * first available high mem area, with some guard pages
 * for adjacent stacks.
 */
void
vmm_thread_init(thread_t * thread, 
		unsigned int user_stack_size,
		unsigned int kernel_stack_size)
{
  //MAGIC_BREAK;			

  task_t * task = thread->task;
  pde_t * page_dir = (pde_t *)task->page_dir;
  void * ppf = NULL;
  unsigned int num_pages = ((user_stack_size - 1) >> PAGE_SHIFT) + 1;
  
  lprintf("Thread VM setup: page_dir:%p, ustack size: %d, num_pages: %d",
	  page_dir,
	  user_stack_size,
	  num_pages);

  lprintf("About to set up thread kernel stack");

  //MAGIC_BREAK;

  // Create the kernel stack for this thread
  // Also set the thread->esp
  vmm_kernel_stack_init(thread,
			kernel_stack_size);
  
  lprintf("thread ESP: 0x%x", thread->esp);
  lprintf("thread Kernel ESP: 0x%x", thread->kernel_esp);
  
  lprintf("About to set up thread user stack");
  //MAGIC_BREAK;

  // To do: Dynamic allocation for vm area.
  // Leave some space for argc/argv?.
  
  
  ppf = vmm_ppfs_alloc(num_pages);

  
  lprintf("stack_start: %p, num_pages: %u, ppf: %p",
	  (void *)(thread->esp - (num_pages << PAGE_SHIFT) + 1),
	  num_pages,
	  ppf);
  
  
  vmm_map_pages(page_dir,
		(void *)(thread->esp - (num_pages << PAGE_SHIFT) + 1),
		ppf,
		num_pages,
		PGTAB_ATTRIB_USR_RW);

  thread->esp -= THREAD_STACK_MARGIN;     
  
  lprintf("vmm_create_stack exit");

}
