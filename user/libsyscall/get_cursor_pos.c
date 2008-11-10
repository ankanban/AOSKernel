
#include <syscall_int.h>
#include <syscall_wrapper.h>
#include <simics.h>

int
get_cursor_pos(int *row, int *col)
{
  return syscall_wrapper_2(GET_CURSOR_POS_INT, 
			   2,
			   (unsigned int)row,
			   (unsigned int)col);
}
