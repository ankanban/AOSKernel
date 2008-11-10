#include <stddef.h>
#include <malloc.h>
#include <vmm.h>

/* safe versions of malloc functions */
void *malloc(size_t size)
{
  int i = 0;
  void * addr = NULL;
  int num_pages = ((size - 1) >> PAGE_SHIFT) + 1;
  for (i = 0; i < num_pages; i++) {
    if (i == 0) {
      addr = vmm_ppf_alloc();
    } else {
      vmm_ppf_alloc();
    }
  }
  return addr;
}

void *memalign(size_t alignment, size_t size)
{
  return NULL;
}

void *calloc(size_t nelt, size_t eltsize)
{
  return NULL;
}

void *realloc(void *buf, size_t new_size)
{
  return NULL;
}

void free(void *buf)
{
  return;
}

void *smalloc(size_t size)
{
  return NULL;
}

void *smemalign(size_t alignment, size_t size)
{
  return NULL;
}

void *scalloc(size_t size)
{
  return NULL;
}

void sfree(void *buf, size_t size)
{
  return;
}


