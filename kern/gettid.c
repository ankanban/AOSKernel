
#include <syscall_wrapper.h>

int 
gettid(void)
{
  return syscall_wrapper(GETTID_INT, 
			 0);
}
