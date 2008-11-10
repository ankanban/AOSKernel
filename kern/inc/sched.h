#ifndef __SCHED_H__
#define __SCHED_H__

#include <task.h>

void
sched_yield(void);

thread_t *
sched_get_current_thread(void);

void
sched_set_current_thread(thread_t * thread);

#endif /* __SCHED_H__ */
