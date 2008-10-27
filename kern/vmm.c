
#include <vmm.h>

int page_count;

/**
 * 
 * Just return the first available page
 */
void *
vmm_ppf_alloc(void)
{
  void * ppf = USER_MEM_BASE + (page_count << PAGE_SHIFT); 
  memset(ppf, 
	 0, 
	 PAGE_SIZE);
  return ppf;
}

void
vmm_ppf_free(void * page_addr)
{
  /* TODO: Empty stub */
  return;
}

void *
vmm_pt_alloc(void)
{
  return vmm_ppf_alloc();
}

static void
vmm_create_code_segment(task_t * task, 
			unsigned int pd_index)
{
  pde_t * page_dir = (pde_t *)task->page_dir;
  pte_t * pgtable = (pte_t *)vmm_pt_alloc();
  void * start = NULL;
  
  vm_area_init(&task->vma_code);

  /* Should go into vma->create and other memeber functions */

  *((unsigned int *)task->page_dir + pd_index) = (0x101 | 
						  (unsigned int)pgtable);
 
  start = pd_index * PGTAB_REGION_SIZE,

  task->vma_code.create(&task->vma_code,
			start,
			PGTAB_REGION_SIZE,
			VMA_TYPE_RO,
			task);

  for (int i = 0; i < PAGE_SIZE >> 2; i++) {
    void * ppf = vmm_ppf_alloc();
    pgtable[i] = (0x103 | (unsigned int)ppf);
  }

}

/* 
 * Create the virtual memory map for the task
 */
void
vmm_init(task_t * task, 
	 unsigned int code_segment_size)
{
  int i = 0;
  unsigned int cr3 = get_cr3();
  void * kpbdr = (cr3 >> PAGE_SHIFT) << PAGE_SHIFT;
  task->page_dir = vmm_ppf_alloc();
  

  /* Allocate kernel area */
  vm_area_init(&task->vm_kernel);
  /* This should go into vma_create ??? per vma_area*/
  /* XXX Hard coded, should use USER_MEM_START */
  for (i = 0; i < 4;
       i++) {
    *((unsigned int *)task->page_dir + i) = *((unsigned int *)kpbdr + i);
    /* XXX Mark as read-only */   
  }

  /* Guard Area between kernel area & next segment */

  i++;

  /* XXX Allocate code area, hard coded to one page table (4MB) */

  vmm_create_code_segment(task, 
			  i);


  /* XXX Allocate data segment */
  /* No data segment allocated right now, should happen 'on-demand' using
   * malloc from user-space, when we implement demand-paging
   */
  vm_area_init(&task->vma_data);

}

/* Create a 4K Stack at the very top of the
 * address space.
 */
void
vmm_create_stack(thread_t * thread, 
		 unsigned int size)
{
  task_t * task = thread;
  pde_t * page_dir = (pde_t *)task->page_dir;
  pte_t * pgtable = (pte_t *)vmm_pt_alloc();
  void * ppf = vmm_ppf_alloc();
  
  page_dir[1023] = (0x101 | (unsigned int)pgtable);
  
  pgtable[1023] = (0x103 | (unsigned int)ppf);

  thread->stack_ptr = 0xffffffff;
  
}
