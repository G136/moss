#include "general.h"

/*
#ifdef LOAD_ADDRESS
#ifdef KERNEL_SIZE
#define ALLOC_ARENA_START LOAD_ADDRESS + KERNEL_SIZE
#define ALLOC_ARENA_LIMIT 0x80_000
#endif
#endif
*/

#define ALLOC_ARENA_START 0x100000
#define ALLOC_ARENA_LIMIT 0x200000

void* alloc_forever(dword size, dword alignment);
