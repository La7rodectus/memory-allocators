#define DEFAULT_ALLOCATOR_ARENA_PAGES 16

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

  #include <unistd.h>
  #define DEFAULT_ALLOCATOR_ARENA_PAGESIZE (size_t)(sysconf(_SC_PAGESIZE))
  #define _IS_UNIX 1
#else

  #define DEFAULT_ALLOCATOR_ARENA_PAGESIZE 4096
  #define _IS_WIN 1

#endif

#define ARENA_SIZE (DEFAULT_ALLOCATOR_ARENA_PAGESIZE * DEFAULT_ALLOCATOR_ARENA_PAGES)

