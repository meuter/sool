#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;

class_t *Exception(); /* new(Exception(), "some message") */
class_t *NullPointerError();
class_t *IndexError();

int __exc_c;

#define try                 for (__exc_c = 0; __exc_c == 0; (void)__exception_pop(), __exc_c = 1) if (setjmp(*__exception_push()) == 0)
#define throw(e)            __exception_throw(e)
#define catch(class, e)     else if ( class_of(__exception_top()) == class && (e = __exception_catch()))
#define except              else if ( (__exception_catch()) )


jmp_buf *__exception_push();
void     __exception_throw(void *something);
void    *__exception_pop();
void    *__exception_catch();
void    *__exception_top();

#endif
