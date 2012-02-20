#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <sool/io.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;

class_t *Exception(); /* new(Exception(), "error message") */

int __exc_c;

#define try					for (__exc_c = 0; __exc_c == 0; exception_throw_uncaught(), __exc_c = 1)	\
								if (setjmp(*exception_push()) == 0)										\
									for (__exc_c = 0; __exc_c == 0; (void)exception_pop(), __exc_c = 1)
#define throw(e)			exception_throw(e)
#define catch(class, e)		else if (is_a(class, exception_top()) && (e = exception_pop()))
#define except				else if (exception_pop())

// TODO: make exception stacktrace thread local

jmp_buf *exception_push();
void     exception_throw(void *something);
void     exception_throw_uncaught();
void    *exception_top();
void    *exception_pop();

#endif
