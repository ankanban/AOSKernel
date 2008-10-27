

int gettid(void);

void
task_idle()
{
  int tid = gettid();

  lprintf("TID: %d", tid);

  while (1) {
    continue;
  }
}
