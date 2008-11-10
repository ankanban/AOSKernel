#ifndef __SYSCALL_WRAPPER_H__
#define __SYSCALL_WRAPPER_H__

#include <syscall_int.h>

int syscall_wrapper_0(unsigned long sys_int_num,
		      unsigned int arg_num);
int syscall_wrapper_1(unsigned long sys_int_num,
		      unsigned int arg_num,
		      unsigned long arg1);
int syscall_wrapper_2(unsigned long sys_int_num,
		      unsigned int arg_num,
		      unsigned long arg1,
		      unsigned long arg2);
int syscall_wrapper_3(unsigned long sys_int_num,
		      unsigned int arg_num,
		      unsigned long arg1,
		      unsigned long arg2,
		      unsigned long arg3);

int
syscall_wrapper(unsigned long sys_int_num,
		unsigned long num_args,
		...);


#endif /* __SYSCALL_WRAPPER_H__ */
