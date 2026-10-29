#include "alloc.h"
#include "panic-and-assert.h"


dword alloc_progress = ALLOC_ARENA_START;

void* alloc_forever(dword size, dword alignment) {
  dword addr = alloc_progress & -alignment;
  alloc_progress = addr + size;

  ASSERT(alloc_progress < ALLOC_ARENA_LIMIT);
  
  return (void*) addr;
}
