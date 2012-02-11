#ifndef __MEM_H__
#define __MEM_H__

#include <stddef.h>
#include <sool/object.h>

class_t *MemoryError();

void *mem_alloc  (size_t size);
void  mem_free    (void *ptr);
void *mem_realloc (void *ptr, size_t size);

#endif
