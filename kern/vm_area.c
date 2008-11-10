#include <task.h>
#include <thread.h>
#include <vm_area.h>
#include <string.h>
#include <elf_410.h>

void
vm_area_create_identity(vm_area_t * vma,
			void * start,
			uint32_t size,
			uint32_t flags,
			task_t * task)
{
  void * start_vpf = (void *)((uint32_t)start & PAGE_MASK);
  void * end_vpf = (void *)(((uint32_t)start + size - 1) & PAGE_MASK);
  void * vpf = NULL;
  int i = 0;
  int num_pages = ((uint32_t)(end_vpf - start_vpf) >> PAGE_SHIFT) + 1;
  
  lprintf("identity spage: %p, epage: %p, vmarealen: 0x%x, num_pages: 0x%x",
   	  start_vpf,
	  end_vpf,
	  size, 
	  num_pages);
   

  //MAGIC_BREAK;

  for (i = 0; i < num_pages; i++) {
    vpf = start_vpf + (i << PAGE_SHIFT);
  
    vmm_map_page(task->page_dir,
		 vpf,
		 vpf,
		 flags);
    /*
     *lprintf("vmarea_ident: N task %d, vpf:%p, ppf:%p, flags: %0x%x",
     *    task->id,
     *vpf,
     *    vpf,
     *    flags);
     */
  }

}

void
vm_area_create(vm_area_t * vma,
	       void * start,
	       unsigned int size,
	       uint32_t flags,
	       task_t * task)
{

  // for vma_area's
  void * start_vpf = (void *)((uint32_t)start & PAGE_MASK);
  void * end_vpf = (void *)(((uint32_t)start + size - 1) & PAGE_MASK);
  void * vpf = NULL;
  int i = 0;
  int num_pages = ((uint32_t)(end_vpf - start_vpf) >> PAGE_SHIFT) + 1;

  lprintf("vmarealen: %u, pages: %u",
	  size, 
	  num_pages);

  
  uint32_t mapping = 0;

  for (i = 0; i < num_pages; i++) {
    vpf = start_vpf + (i << PAGE_SHIFT);

    if ((mapping = vmm_get_mapping(task->page_dir, 
				   vpf)) == 0) {

      void * ppf = vmm_ppf_alloc();

      vmm_map_page(task->page_dir,
		   vpf,
		   ppf,
		   flags);

      lprintf("vmarea: N task %d, vpf:%p, ppf:%p, flags: 0x%x",
	      task->id,
	      vpf,
	      ppf,
	      flags);

    } else { 
      // replace page attributes
      vmm_map_page(task->page_dir,
		   vpf,
		   (void *)(mapping & PAGE_MASK),
		   flags);
      lprintf("vmarea: M task %d, vpf:%p, ppf:%p, flags: 0x%x",
	      task->id,
	      vpf,
	      (void *)(mapping & PAGE_MASK),
	      flags);
      
    }

  }

  if (vma != NULL) {
    vma->start_address = start;
    vma->size = size;
    vma->flags = flags;
    vma->task = task;
  }
}

void
vm_area_zero(struct vm_area * vma,
	     void * addr,
	     uint32_t length)
{
  if (vma->start_address > addr ||
      vma->start_address + vma->size < addr + length) {
    // Out of bounds
    return;
  }
  void * start_vpf = (void *)((uint32_t)addr & PAGE_MASK);
  void * end_vpf = (void *)(((uint32_t)addr + length - 1) & PAGE_MASK);
  int num_pages = ((uint32_t)(end_vpf - start_vpf) >> PAGE_SHIFT) + 1;
  uint32_t pg_offset = ((uint32_t)addr & PAGE_DIV);
  int total_bytes_zeroed = 0;
  int i = 0;

  for (i = 0; i < num_pages; i++) {
    int bytes_to_zero = length - total_bytes_zeroed;
    void * vpf = start_vpf + (i << PAGE_SHIFT);
    void * ppf = (void *)((uint32_t)vmm_get_mapping(vma->task->page_dir, 
						    vpf) & PAGE_MASK);

    if (pg_offset + bytes_to_zero > PAGE_SIZE) {
      bytes_to_zero = PAGE_SIZE - pg_offset;
    }
    
    memset(ppf + pg_offset, 0, bytes_to_zero);
    
    lprintf("Vmazero: vpf: %p, ppf: %p, pgoff: 0x%x, zeroed_bytes: 0x%x",
	    vpf,
	    ppf,
	    pg_offset,
	    bytes_to_zero);
    
    pg_offset = 0;
  }
}

int
vm_area_load(struct vm_area * vma,
	     const char * fname,
	     uint32_t foffset,
	     uint32_t length,
	     void * load_addr)
{

  if (vma->start_address > load_addr ||
      vma->start_address + vma->size < load_addr + length) {
    // Out of bounds
    return -1;
  }

  int i = 0;
  int bytes_read = 0;
  int total_bytes_read = 0;
  void * start_vpf = (void *)((uint32_t)load_addr & PAGE_MASK);
  void * end_vpf = (void *)(((uint32_t)load_addr + length - 1) & PAGE_MASK);

  int num_pages = ((uint32_t)(end_vpf - start_vpf) >> PAGE_SHIFT) + 1;
  uint32_t load_offset = ((uint32_t)load_addr & PAGE_DIV);
  
  for (i = 0; i < num_pages; i++) {
    int bytes_to_read = length - total_bytes_read;
    void * vpf = start_vpf + (i << PAGE_SHIFT);
    void * ppf = (void *)((uint32_t)vmm_get_mapping(vma->task->page_dir, 
						    vpf) & PAGE_MASK);

    if (load_offset + bytes_to_read > PAGE_SIZE) {
      bytes_to_read = PAGE_SIZE - load_offset;
    }

    bytes_read = getbytes(fname,
			  foffset + total_bytes_read,
			  bytes_to_read,
			  ppf + load_offset);

    lprintf("Vmaread: vpf: %p, ppf: %p, foff: 0x%x, pgoff: 0x%x, reqbytes: 0x%x, readbytes: 0x%x",
	    vpf,
	    ppf,
	    foffset + total_bytes_read,
	    load_offset,
	    bytes_to_read,
	    bytes_read);

    if (bytes_read < 0) {
      // read error
      return bytes_read;
    } else if (bytes_read == 0 || 
	       bytes_read != bytes_to_read) {
      // XXX truncated read
      return -1;
    } 

    load_offset = 0;
    total_bytes_read += bytes_read;

  }
  
  return total_bytes_read;
}

#if 0
static void
vm_area_add_page(vm_area_t * vma,
		 void * pfaddr)
{
  
  return;
}
#endif

void
vm_area_init(vm_area_t * vma)
{
  memset(vma, 0, sizeof(vm_area_t));
  //vma->create = vm_area_create;
  //vma->add_page = vm_area_add_page;
}
