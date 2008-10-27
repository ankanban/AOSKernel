#ifndef __THREAD_H__
#define __THREAD_H__

struct task;
typedef struct task task_t;

typedef struct {
  unsigned int id;

  /* Stack state */
  vm_area_t vm_stack;
  void * stack_ptr;
  unsigned int stack_segment;

  task_t * task;
  /* List ptr for thread list */
} thread_t;

#endif
