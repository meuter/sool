#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;

class_t *Exception(); /* new(Exception()) */

int __exc_c;

#define try              for (__exc_c = 0; __exc_c == 0; (void)exception_pop(), __exc_c = 1) if (setjmp(*exception_push()) == 0)
#define throw(e)         exception_throw(e)
#define catch(class, e)  else if ( class_of(exception_top()) == class && (e = exception_catch()))
#define except           else if ( (exception_catch()) )


jmp_buf *exception_push();
void     exception_throw(void *something);
void    *exception_pop();
void    *exception_catch();
void    *exception_top();

#endif
