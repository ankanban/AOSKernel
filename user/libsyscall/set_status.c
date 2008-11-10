
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

void 
set_status(int status)
{
  syscall_wrapper_1(GETTID_INT, 
		    1, 
		    status);
}
