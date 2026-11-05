#include "cpu.h"
#include "printing.h"

#define PANIC(...)		    \
  cpu_cli();			    \
  /* window_clear(); */		    \
  print("!!!! ");		    \
  printf(__VA_ARGS__);		    \
  cpu_hlt();

//#ifdef DEBUG
#define ASSERT(cond)						\
  if (!(cond)) {						\
    PANIC("ASSERT(%s) at %s:%d", #cond, __FILE__, __LINE__);	\
  }
//#else
//#define ASSERT(cond)
//#endif
