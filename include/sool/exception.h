#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <sool/io.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;


struct _stack_frame_t;
typedef struct _stack_frame_t stack_frame_t;

typedef enum { __TRY, __CATCH, __FINALLY } frame_stage_t;

class_t *Exception(); /* new(Exception(), "error message") */

int __exc_c;

#if 1
#define try					for (__exc_c = 0; __exc_c == 0; exception_throw_uncaught(), __exc_c = 1)	\
								if (setjmp(*exception_push()) == 0)										\
									for (__exc_c = 0; __exc_c == 0; (void)exception_pop(), __exc_c = 1)
#define throw(e)			exception_throw(e)
#define catch(class, e)		else if (is_a(class, exception_top()) && (e = exception_pop()))
#define except				else if (exception_pop())

#else

#define try					for (__exc_c = 0; __exc_c == 0; frame_pop(), __exc_c = 1)	 		 				\
								if (setjmp(*frame_push()) == 0)													\
									for (;;frame_set_stage(frame_top(), __FINALLY), frame_jmp(frame_top()))
#define throw(e)			frame_throw(e)
#define catch(class, e)		else if (frame_get_stage(frame_top()) == __CATCH && is_a(class, frame_get_thrown(frame_top())) && (e = frame_get_thrown(frame_top()))) for (;;frame_set_stage(frame_top(), __FINALLY), frame_jmp(frame_top()))
#define except				else if (frame_get_stage(frame_top()) == __CATCH) for (;;frame_set_stage(frame_top(), __FINALLY), frame_jmp(frame_top()))
#define finally				else if (frame_get_stage(frame_top()) == __FINALLY)

#endif

jmp_buf *frame_push();
stack_frame_t *frame_top();
void frame_pop();
void frame_jmp(stack_frame_t *frame);
void *frame_get_thrown(stack_frame_t *frame);
void frame_set_stage(stack_frame_t *frame, frame_stage_t stage);
frame_stage_t frame_get_stage(stack_frame_t *frame);
void frame_throw(void *something);


// TODO: make exception stacktrace thread local
// TODO: add finally support
// TODO: add rethrow() akin to raise in python
// TODO: catch signal in a try { fndgkskdfgjklg } catch block

jmp_buf *exception_push();
void     exception_throw(void *something);
void     exception_throw_uncaught();
void    *exception_top();
void    *exception_pop();




#endif
