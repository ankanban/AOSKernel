
#include <sched.h>

thread_t * current_thread = NULL;

void
sched_yield(void)
{
  thread_switch(current_thread);
}

thread_t *
sched_get_current_thread(void)
{
  return current_thread;
}

void
sched_set_current_thread(thread_t * thread)
{
  current_thread = thread;
}

void
sched_save_state(uint32_t eip, 
		  uint32_t esp)
{
  current_thread->eip = eip;
  current_thread->esp = esp;
}
