#ifndef __KERNEL_ASM_H__
#define __KERNEL_ASM_H__

#include <x86/seg.h>

#define SYSCALL_PROLOGUE \
  do {			 \
    set_kernel_ds();	 \
    set_kernel_ss();	 \
    set_kernel_pgdir();	 \
} while (0)

#define DEFINE_SET_SEG_PROTO(reg)		\
  void set_kernel_##reg(void);			\
  void set_user_##reg(void);

#define DEFINE_ALL_SET_SEG_PROTO \
  DEFINE_SET_SEG_PROTO(ds)	 \
  DEFINE_SET_SEG_PROTO(es)	 \
  DEFINE_SET_SEG_PROTO(fs)	 \
  DEFINE_SET_SEG_PROTO(gs)	 \
  DEFINE_SET_SEG_PROTO(ss)        


#define DEFINE_SET_SEG(seg, reg)		\
  .global set_kernel_##reg;			\
set_kernel_##reg:				\
 pusha;						\
 mov $SEGSEL_KERNEL_##seg, %ax;			\
 mov %ax, %##reg;				\
 popa;						\
 ret;                                           \
 .global set_user_##reg;			\
set_user_##reg:	   			        \
 mov $SEGSEL_USER_##seg, %ax;			\
 mov %ax, %##reg;				\
 popa;						\
 ret;

#define DEFINE_ALL_SET_SEG			  \
  DEFINE_SET_SEG(DS, ds);			  \
  DEFINE_SET_SEG(DS, es);			  \
  DEFINE_SET_SEG(DS, fs);			  \
  DEFINE_SET_SEG(DS, gs);                         \
  DEFINE_SET_SEG(DS, ss);                         



#endif /*__KERNEL_ASM_H__ */
