#ifndef __EXCEPTION_WRAPPER_H__
#define __EXCEPTION_WRAPPER_H__

#define ERRFLAG_OFFSET 1

#define DECL_EXCEPTION_WRAPPER(name) \
  void name##_exception_wrapper(void)

#define DEFINE_EXCEPTION_WRAPPER(name)          \
  .global name##_exception_wrapper;		\
  name##_exception_wrapper:                     \
  movl %esp, %ebp;				\
  pusha;					\
  movl (%ebp, ERRFLAG_OFFSET), %ebx;		\
  pushl %ebx;					\
  call name##_exception_handler;		\
  popl %ebx;					\
  popa;						\
  iret;                                     


#endif /* __EXCEPTION_WRAPPER_H__ */
