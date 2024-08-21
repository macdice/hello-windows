#include <assert.h>
#include <stdbool.h>
#define HAVE_PTHREAD_BARRIER_WAIT
#ifdef _WIN32
#define WIN32
#endif
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Assert(x) assert(x)
#define USE_ASSERTION_CHECKING
#define pg_read_barrier()
#define pg_write_barrier()
