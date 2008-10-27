#ifndef __SYSCALL_WRAPPER_H__
#define __SYSCALL_WRAPPER_H__

#include <syscall_int.h>

int
syscall_wrapper(unsigned long sys_int_num,
		unsigned long num_args,
		...);


#endif /* __SYSCALL_WRAPPER_H__ */
