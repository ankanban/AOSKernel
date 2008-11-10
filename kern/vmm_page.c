#include <stddef.h>
#include <string.h>
#include <x86/page.h>
#include <x86/cr.h>
#include <vmm_page.h>
#include <common_kern.h>
#include <simics.h>
#include <stdint.h>

pde_t * kernel_page_dir;

int page_count;
void * user_ppf_start = NULL;
void * highmem_ppf_start = (void *)~0UL;
int highmem_page_count = 1;
void * user_highmem_start = (void *)~0UL;

void *
vmm_ppf_highmem_alloc(void)
{
  void * ppfhm = (void *)(highmem_ppf_start - 
			  (highmem_page_count << PAGE_SHIFT) + 1);
  lprintf("Allocated HIGHMEM Page: %p", ppfhm);
  //MAGIC_BREAK;

  memset(ppfhm,
	 0,
	 PAGE_SIZE);

  highmem_page_count++;
  return ppfhm + (PAGE_SIZE - 1);
}

void *
vmm_ppfs_highmem_alloc(int num_pages)
{
  int i = 0;
  void * ppfhm_addr = NULL;
  for (; i < num_pages; i++) {
    //MAGIC_BREAK;
    if (i == 0) {
      ppfhm_addr = vmm_ppf_highmem_alloc();
    } else {
      vmm_ppf_highmem_alloc();
    }
  }
  return ppfhm_addr;
}

/**
 * 
 * Just return the first available page
 */
void *
vmm_ppf_alloc(void)
{
  void * ppf = (void *)(user_ppf_start + (page_count << PAGE_SHIFT)); 
  /* XXX Page allocation should not reset memory */
  memset(ppf, 
	 0, 
	 PAGE_SIZE);
  /** If out of pages, and a user request, yield to other process
   */
  /** If a kernel request, something bad has happened */
  page_count++;
  return ppf;
}

void *
vmm_ppfs_alloc(int num_pages)
{
  int i = 0;
  void * ppf_addr = NULL;
  for (; i < num_pages; i++) {
    if (i == 0) {
      ppf_addr = vmm_ppf_alloc();
    } else {
      vmm_ppf_alloc();
    }
  }
  return ppf_addr;
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

pde_t *
vmm_create_tables(int num_pdes)
{
  pde_t * page_dir = (pde_t *)vmm_ppf_alloc();
  int i = 0;

  lprintf("Creating pg tables, num_pdes: 0x%x, page_dir: %p",
	  num_pdes,
	  page_dir);
  
  
  // First allocate memory for all of the page tables.
  for (i = 0; i < num_pdes; i++) {
    
    pte_t * page_table = (pte_t *)vmm_ppf_alloc();
    
    page_dir[i].value = PGDIR_ATTRIB_SU_RW | (unsigned long)page_table;
  }
 
  return page_dir;
}

void 
vmm_map_pages(pde_t * page_dir,
	      void * vaddr,
	      void * ppf,
	      int num_pages,
	      int flags)
{
  int i = 0;
  for (i = 0; i <  num_pages; i++) {
    vmm_map_page(page_dir,
		 vaddr + (i << PAGE_SHIFT),
		 ppf + (i << PAGE_SHIFT),
		 flags);
  }

}

uint32_t
vmm_get_mapping(pde_t * page_dir,
		void * vaddr)
{
  pte_t * pte = NULL;
  pde_t * pde = VMM_GET_PDE(page_dir,
			    vaddr);
  
  if (pde->value == 0) {
    /* Should never happen */
    return 0;
  }
  
  pte = VMM_GET_PTE(page_dir, vaddr);
  
  return pte->value;
}

void
vmm_map_page(pde_t * page_dir,
	     void * vaddr,
	     void * ppf,
	     int flags)
{
  pte_t * pte = NULL;
  pde_t * pde = VMM_GET_PDE(page_dir,
			    vaddr);

  if (pde->value == 0) {
    // Allocate a page table
    return;
  } else {
    // Only toggle the flags, replacing with newer attributes
    pde->value = (flags | (uint32_t)(pde->value & PAGE_MASK));
  }

  pte = VMM_GET_PTE(page_dir, vaddr);

  
  // Replace previous mapping
  pte->value = (flags | (uint32_t)ppf);
 
}

void
vmm_set_kernel_pgdir(void)
{
  // Set the kernel page directory
  set_cr3((uint32_t)kernel_page_dir);
}

void
vmm_page_init()
{
  unsigned int num_ppf = machine_phys_frames();
  unsigned int num_pdes = VMM_NUM_PDES(num_ppf);

   // Set up high-mem page allocation
  highmem_ppf_start = (void *)((num_ppf << PAGE_SHIFT) - 1);

  lprintf("Highmem start: %p", highmem_ppf_start);

  // Set up low-mem page allocation state.
  user_ppf_start = (void *)(USER_MEM_START);
  lprintf("Lowmem start: %p", user_ppf_start);

  lprintf("Physical frames       : 0x%x", num_ppf);
  lprintf("PDEs for Phys. Frames : 0x%x", num_pdes);
 
  pde_t * page_dir = vmm_create_tables(num_pdes);

  // Now map the Kernel physical address
  // directly into virtual address range
  vmm_map_pages(page_dir,
		0,
		0,
		num_ppf,
		PGTAB_ATTRIB_SU_RW);
  
  lprintf("Created Page tables, about to enable paging");

  //MAGIC_BREAK;

  kernel_page_dir  = page_dir;

  vmm_set_kernel_pgdir();


  // Turn Paging on
  uint32_t cr0 = get_cr0();

  cr0 |= CR0_PG;

  set_cr0(cr0);

}
