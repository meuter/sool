#include <sool/mem.h>
#include <sool/exception.h>

#include <string.h>
#include <assert.h>

#include "exception_def.h"

#define __USE_GC__

#ifdef __USE_GC__

#include <gc/gc.h>
#define MALLOC  GC_MALLOC
#define FREE    GC_FREE
#define REALLOC GC_REALLOC

#else

#include <malloc.h>
#define MALLOC  malloc
#define FREE    free
#define REALLOC realloc

#endif

DEFINE_EXCEPTION(MemoryError);

void *mem_alloc(size_t size) {
	void *result = MALLOC(size);
	if (result == NULL)	throw(new(MemoryError(), ""));
	return result;
}

void mem_free(void *ptr) {
	FREE(ptr);
}

void *mem_realloc(void *ptr, size_t size) {
	void *result = REALLOC(ptr, size);
	if (result == NULL)	throw(new(MemoryError(), ""));
	return result;
}
