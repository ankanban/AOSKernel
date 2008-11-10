#ifndef __VM_AREA_H__
#define __VM_AREA_H__

#include <stdint.h>
#include <stddef.h>
#include <simics.h>
#include <x86/page.h>
#include <vmm_page.h>

struct task;

typedef enum {
  VMA_TYPE_RO,
  VMA_TYPE_RW
} vma_type_t;

#define VMA_PTE_ATTRIB 0x101

struct vm_area;
//typedef struct vm_area vm_area_t;

typedef struct vm_area {
  void * start_address;
  unsigned int size;
  unsigned int flags;
  unsigned int type;
  struct task * task;

  void (*create)(struct vm_area * vma, 
		 void * start, 
		 unsigned int size,
		 unsigned int type,
		 struct task * task);

  void (*add_page)(struct vm_area * vma, 
		   void * pfaddr);
  
  /* Reclaim */
  /* Split */
  /* Merge */

  /* List pointers */
} vm_area_t;

void
vm_area_init(vm_area_t * vma);

void
vm_area_create_identity(vm_area_t * vma,
			void * start,
			uint32_t size,
			uint32_t flags,
			struct task * task);

void
vm_area_create(vm_area_t * vma,
	       void * start,
	       unsigned int size,
	       uint32_t flag,
	       struct task * task);
int
vm_area_load(struct vm_area * vma,
	     const char * fname,
	     uint32_t foffset,
	     uint32_t length,
	     void * load_addr);

void
vm_area_zero(struct vm_area * vma,
	     void * addr,
	     uint32_t length);

#endif
