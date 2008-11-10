
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
print(int size, char * buf)
{
  return syscall_wrapper_2(PRINT_INT, 
			   2,
			   (unsigned int)size,
			   (unsigned int)buf);
}
