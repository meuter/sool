#include <sool/exception.h>
#include <sool/stack.h>
#include <sool/error.h>

#include "object_def.h"

/*****************************************************************************/

struct _exception_t {
	EXTENDS(object_t);
	jmp_buf buff;
	const char *msg;
};

void *exception_ctor(void *_self, va_list *args) {
	exception_t *self = super_ctor(Exception(), _self, args);
	self->msg = va_arg(*args, const char *);
	return self;
}

int exception_put(void *_self, FILE *stream, char *subformat) {
	(void)subformat;
	exception_t *self = cast(Exception(), _self);
	return fprintf(stream, "%s", self->msg);
}

class_t *Exception() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), "Exception", Object(), sizeof(exception_t),
			ctor, exception_ctor,
			put,  exception_put,
			NULL
		);
	}
	return result;
}

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
