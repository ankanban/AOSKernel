
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
wait(int *status_ptr)
{
  return syscall_wrapper_1(GETTID_INT, 
			   1,
			   (unsigned int)status_ptr);
}
