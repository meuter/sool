#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;

class_t *Exception(); /* new(Exception(), "some message") */

#define try                 if (setjmp(*__try()) == 0)
#define catch(class, e)     else if ( class_of(__top()) == class && (e = __catch()) )
#define other(e)            else if ( (e = __catch()) || TRUE )
#define throw(s)            __throw(s)

jmp_buf *__try();
void     __throw(void *something);
void    *__catch();
void    *__top();

#endif
