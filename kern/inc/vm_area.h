#ifndef __VM_AREA_H__
#define __VM_AREA_H__

struct task;
typedef struct task task_t;

typdef enum {
  VMA_TYPE_RO,
  VMA_TYPE_RW
};

struct vm_area;
typedef struct vm_area vm_area_t;

typedef struct vm_area {
  void * start_address;
  unsigned int size;
  unsigned int flags;
  unsigned int type;
  struct task * task;

  void (*create)(vm_area_t * vma, 
		 void * start, 
		 unsigned int size,
		 unsigned int type,
		 task_t * task);

  void (*add_page)(vm_area_t * vma, 
		   void * pfaddr);
  
  /* Reclaim */
  /* Split */
  /* Merge */

  /* List pointers */
} vm_area_t;

void
vm_area_init(vm_area_t * vma);

#endif
