#ifndef __VMM_PAGE_H__
#define __VMM_PAGE_H__


typedef struct pde {
  unsigned int value;
}  __attribute__((packed)) pde_t;

typedef struct pte {
  unsigned int value;
} __attribute__((packed)) pte_t;



#define PAGE_DIV (PAGE_SIZE -1)
#define PAGE_MASK ~(PAGE_SIZE - 1)

#define PGDIR_ATTRIB_SU_RO 0x001
#define PGDIR_ATTRIB_SU_RW 0x003

#define PGDIR_ATTRIB_USR_RO 0x005
#define PGDIR_ATTRIB_USR_RW 0x007

#define PGTAB_ATTRIB_SU_RO 0x001
#define PGTAB_ATTRIB_SU_RW 0x003

#define PGTAB_ATTRIB_USR_RO 0x005
#define PGTAB_ATTRIB_USR_RW 0x007

#define PDE_SHIFT 2
#define PGDIR_SIZE (PAGE_SIZE >> PDE_SHIFT)
#define PGDIR_SHIFT (PAGE_SHIFT - PDE_SHIFT)


#define PTE_SHIFT 2
#define PGTAB_SIZE (PAGE_SIZE >> PTE_SHIFT)
#define PGTAB_SHIFT (PAGE_SHIFT - PTE_SHIFT)


/* Sizes covered by a page table struct */
#define PGTAB_REGION_SHIFT (PGTAB_SHIFT + PAGE_SHIFT)  
#define PGTAB_REGION_SIZE (1 << PGTAB_REGION_SHIFT)
#define PGTAB_REGION_DIV (PGTAB_REGION_SIZE - 1)
#define PGTAB_REGION_MASK (~PGTAB_REGION_DIV)


#define PDE_ENTRY_MASK 0xffc00000
#define PDE_ENTRY_SHIFT 22
#define PTE_ENTRY_MASK 0x003ff000
#define PTE_ENTRY_SHIFT 12

#define VMM_PPN2PPF(page_num) ((page_num) << PAGE_SHIFT)
#define VMM_NUM_PDES(num_pages) ((VMM_PPN2PPF((num_pages) - 1)	\
				  >> PDE_ENTRY_SHIFT) + 1)

// Returns a ptr to the page directory entry for teh given virtual address
#define VMM_GET_PDE(pgdir, vaddr) ((pde_t *)(pgdir) + (((uint32_t)(vaddr) & \
						      PDE_ENTRY_MASK) >> \
						     PDE_ENTRY_SHIFT))

// Returns a ptr to the page tablefor the given virtual address
#define VMM_GET_PGTAB(pgdir, vaddr) ((pte_t *)(VMM_GET_PDE(pgdir,	   \
							   vaddr)->value & \
					       PAGE_MASK))

// Returns a ptr to the page tabel entry for the given virtual address
#define VMM_GET_PTE(pgdir, vaddr) ((pte_t *)(VMM_GET_PGTAB(pgdir, vaddr) + \
					     (((uint32_t)vaddr &	\
					       PTE_ENTRY_MASK) >> \
					      PTE_ENTRY_SHIFT)))

// Returns the physical page frame for a given virtual address
#define VMM_GET_PPF(pgdir, vaddr)  ((void *)(VMM_GET_PTE(pgdir,          \
							 vaddr)->value & \
					     PAGE_MASK))

void *
vmm_ppf_alloc(void);

void *
vmm_ppfs_alloc(int num_pages);

void
vmm_ppf_free(void *);

void *
vmm_ppf_highmem_alloc(void);

void *
vmm_ppfs_highmem_alloc(int num_pages);

uint32_t
vmm_get_mapping(pde_t * page_dir,
		void * vaddr);

void
vmm_map_pages(pde_t * page_dir,
	      void * vaddr,
	      void * ppf,
	      int num_pages,
	      int flags);

void
vmm_map_page(pde_t * page_dir,
	     void * vaddr,
	     void * ppf,
	     int flags);

void 
vmm_map_pages(pde_t * page_dir,
	      void * vaddr,
	      void * ppf,
	      int num_pages,
	      int flags);

pde_t *
vmm_create_tables(int num_pdes);

void
vmm_page_init();

#endif /* __VMM_PAGE_H__ */
