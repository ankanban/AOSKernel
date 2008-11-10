
#include <handler_install.h>
#include <kernel_exceptions.h>
#include <stddef.h>

#define NUM_HANDLERS 20
static void (*exception_wrappers[])(void) = {
  div_exception_wrapper, //0
  dbg_exception_wrapper, //1
  generic_exception_wrapper, //2
  generic_exception_wrapper, //3
  ovf_exception_wrapper, //4
  generic_exception_wrapper, //5
  inv_exception_wrapper, //6
  generic_exception_wrapper, //7
  dbl_exception_wrapper, //8
  generic_exception_wrapper, //9
  generic_exception_wrapper, //10
  seg_exception_wrapper, //11
  stk_exception_wrapper, //12
  gpf_exception_wrapper, // 13
  pgf_exception_wrapper, // 14
  NULL, //15
  generic_exception_wrapper, //16
  generic_exception_wrapper, //17
  generic_exception_wrapper, // 18
  generic_exception_wrapper, // 19
};

void
generic_exception_handler(uint32_t errflag)
{
  lprintf("Exception received: 0x%x", errflag);
}

void
exceptions_init(void)
{

  int i = 0;
  for (i = 0; i < NUM_HANDLERS; i++) {
    set_exception_entry(i, exception_wrappers[i]);
  }
  
  return;
}


void 
div_exception_handler(uint32_t errflag)
{
  lprintf("div Exception received: 0x%x", errflag);
}


void
dbg_exception_handler(uint32_t errflag)
{
  lprintf("dbg Exception received: 0x%x", errflag);
}

void
ovf_exception_handler(uint32_t errflag)
{
  lprintf("ovf Exception received: 0x%x", errflag);
}

void
inv_exception_handler(uint32_t errflag)
{
  lprintf("inv Exception received: 0x%x", errflag);
}

void
dbl_exception_handler(uint32_t errflag)
{
  lprintf("dbl Exception received: 0x%x", errflag);
}

void
seg_exception_handler(uint32_t errflag)
{
  lprintf("seg Exception received: 0x%x", errflag);
}

void
stk_exception_handler(uint32_t errflag)
{
  lprintf("stk Exception received: 0x%x", errflag);
}


void
gpf_exception_handler(uint32_t errflag)
{
  lprintf("gpf Exception received: 0x%x", errflag);
}


void
pgf_exception_handler(uint32_t errflag)
{
  lprintf("pgf Exception received: 0x%x", errflag);
}


