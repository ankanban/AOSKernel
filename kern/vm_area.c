
#include <vm_area.h>

static void
vm_area_create(vm_area_t * vma,
	       void * start,
	       unsigned int size,
	       unsigned int type,
	       task_t * task)
{
  vma->start_address = start;
  vma->size = size;
  vma->type = type;
  vma->task = task;
}

static void
vm_area_add_page(vm_area_t * vma,
		 void * pfaddr)
{
  
  return;
}

void
vm_area_init(vm_area_t * vma)
{
  memset(vma, 0, sizeof(vm_area_t));
  vma->create = vm_area_create;
  vma->add_page = vm_area_add_page;
}
