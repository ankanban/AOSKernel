
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
readline(int size, char * buf)
{
  return syscall_wrapper_2(READLINE_INT, 
			   2,
			   (unsigned int)size,
			   (unsigned int)buf);
}
