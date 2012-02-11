#ifndef __MEM_H__
#define __MEM_H__

#include <stddef.h>

void *xmalloc  (size_t size);
void  xfree    (void *ptr);
void *xrealloc (void *ptr, size_t size);

char *xstrndup(const char *s, size_t n);
char *xstrdup (const char *s);

#endif
