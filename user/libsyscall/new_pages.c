
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
new_pages(void * addr, int len)
{
  return syscall_wrapper_2(NEW_PAGES_INT, 
			   2,
			   (unsigned int)addr,
			   (unsigned int)len);
}
