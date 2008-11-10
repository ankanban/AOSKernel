
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
exec(char * execname, char *argvec[])
{
  return syscall_wrapper_2(GETTID_INT, 
			   2,
			   (unsigned int)execname,
			   (unsigned int)argvec);
}
