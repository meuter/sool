#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <sool/object.h>
#include <sool/io.h>
#include <setjmp.h>

struct _exception_t;
typedef struct _exception_t exception_t;


struct _stack_frame_t;
typedef struct _stack_frame_t stack_frame_t;

typedef enum { __ARMED, __THROWN, __FINISHED } frame_stage_t;

class_t *Exception(); /* new(Exception(), "error message") */

int __exc_c;

#define try					for (__exc_c = 0;__exc_c == 0; frame_pop(), __exc_c = 1)	\
								if (setjmp(*frame_push()) == 0)							\
									for (;; frame_jmp(__FINISHED))
#define throw(e)			frame_throw(e)
#define rethrow()           frame_throw(frame_thrown())
#define catch(class, e)		else if (frame_stage() == __THROWN && (e = frame_thrown()) && is_a(class, e))	\
								for (;; frame_jmp(__FINISHED))
#define except				else if (frame_stage() == __THROWN) for (;; frame_jmp(__FINISHED))
#define finally				else if (frame_stage() == __FINISHED)

jmp_buf       *frame_push();
void           frame_jmp(frame_stage_t stage);
void           frame_throw(void *something);
frame_stage_t  frame_stage();
void          *frame_thrown();
void           frame_pop();

// TODO: make exception stacktrace thread local
// TODO: catch signal in a try { fndgkskdfgjklg } catch block

#endif
