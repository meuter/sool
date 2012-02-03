#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <setjmp.h>

#define try          if (setjmp(*__try()) == 0)
#define catch(e)     else if ( (e = __catch()) || TRUE )
#define throw(s)     __throw(s)

jmp_buf *__try();
void     __throw(void *something);
void    *__catch();
void    *__top();

#endif
