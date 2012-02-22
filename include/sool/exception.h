#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <sool/io.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;

class_t *Exception(); /* new(Exception(), "error message") */
class_t *SegmentationFaultError();

int __exc_c;

#define try					for (__exc_c = 0;__exc_c == 0; exception_pop(), __exc_c = 1)	\
								if (setjmp(*exception_push()) == 0)				  			\
									for (;; exception_jmp(3))
#define throw(e)			exception_throw(e)
#define rethrow()           exception_throw(exception_get())
#define catch(class)		else if (exception_step() == 2 && is_a(class, exception_get()))	\
								for (;; exception_jmp(3))
#define except				else if (exception_step() == 2) for (;; exception_jmp(3))
#define finally				else if (exception_step() == 3)

jmp_buf *exception_push();
void     exception_jmp(int stage);
void     exception_throw(void *something);
int      exception_step();
void    *exception_get();
void     exception_pop();

#endif
