
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
ls(int size, char * buf)
{
  return syscall_wrapper_2(LS_INT, 
			   2,
			   (unsigned int)size,
			   (unsigned int)buf);
}
