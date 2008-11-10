#ifndef __TASK_H__
#define __TASK_H__

#include <vm_area.h>
#include <thread.h>
#include <stdint.h>

/* The Process Control Block (PCB) */
typedef struct task {
  /* Task definition */
  int id;

  void * page_dir;

  /* List of allocated VM regions 
   * XXX Currently hard coded
   */
  vm_area_t vma_kernel;
  vm_area_t vma_code;
  vm_area_t vma_data;

  /* Segment registers */
  unsigned int cs;
  unsigned int ds;

  /* List of threads belonging to this task
   * XXX currently only one thread
   */
  thread_t thread;

} task_t;

void
task_create(task_t * task);

void
task_load(task_t * task,
	  thread_t * thread,
	  char * task_path);
void
do_task_switch(
	       uint32_t eip,
	       uint32_t cs,
	       uint32_t eflags,
	       uint32_t esp,
	       uint32_t ss,
	       uint32_t ds);

#endif /* __TASK_H__ */
