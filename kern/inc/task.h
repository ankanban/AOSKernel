#ifndef __TASK_H__
#define __TASK_H__

#include <thread.h>

/* The Process Control Block (PCB) */
typedef struct task {
  /* Task definition */
  int id;

  void * page_dir;
  unsigned int data_segment;

  /* List of allocated VM regions 
   * XXX Currently hard coded
   */
  vm_area_t vm_kernel;
  vm_area_t vm_code;
  vm_area_t vm_data;

  /* Segment registers */
  unsigned int code_segment;
  unsigned int data_segment;

  /* List of threads belonging to this task
   * XXX currently only one thread
   */
  thread_t thread;

} task_t;

void
task_switch(task_t * task);

void
do_task_switch(void * task_addr,
	       uint32_t eflags,
	       uint32_t cs,
	       uint32_t ss,
	       uint32_t ds);

#endif /* __TASK_H__ */
