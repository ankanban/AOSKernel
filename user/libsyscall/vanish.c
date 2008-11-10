
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

void 
vanish(void)
{
  syscall_wrapper_0(GETTID_INT, 0);
}
