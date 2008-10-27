#ifndef __VMM_H__
#define __VMM_H__

#include <vm_area.h>
#include <task.h>

typedef struct pde {
  unsigned int value;
} pde_t __attribute__((packed));

typedef struct pte {
  unsigned int value;
} pte_t __attribute__((packed));


/* Sizes covered by a page struct */
#define PGTAB_SIZE (PAGE_SIZE >> 2)
#define PGTAB_REGION_SIZE (PGTAB_SIZE << PAGE_SIZE)

void *
vmm_ppf_alloc(void);

void
vmm_ppf_free(void *);

void
vmm_init(task_t * task);

void
vmm_create_stack(thread_t * thread);


#endif /* __VMM_H__ */
