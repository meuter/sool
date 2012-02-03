#include <sool/exception.h>
#include <sool/stack.h>
#include <sool/error.h>

#include "object_def.h"

/*****************************************************************************/

class_t *StackFrame();

typedef struct  {
	EXTENDS(object_t);
	jmp_buf buf;
	void *thrown;
} stack_frame_t;


void *stack_frame_ctor(void *_self, va_list *args) {
	(void)args;
	stack_frame_t *self = cast(StackFrame(), _self);
	self->thrown = NULL;
	return self;
}

class_t *StackFrame() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), "StackFrame", Object(), sizeof(stack_frame_t),
			ctor, stack_frame_ctor,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

static stack_t *stack_trace = NULL;

jmp_buf *__try() {
	if (stack_trace == NULL)
		stack_trace = new(Stack());
	stack_frame_t *e = new(StackFrame());
	stack_push(stack_trace, e);
	return &e->buf;
}

void __throw(void *something) {
	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		assertf(FALSE, "Uncaught exception!");
	}
	stack_frame_t *e = stack_top(stack_trace);
	e->thrown = something;
	longjmp(e->buf, 0);
}

void *__catch() {
	stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
	return frame->thrown;
}

void *__top() {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	return frame->thrown;
}
