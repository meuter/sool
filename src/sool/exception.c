#include <sool/stack.h>
#include <sool/error.h>
#include <sool/io.h>

#include "exception_def.h"

/*****************************************************************************/

void *exception_ctor(void *_self, va_list *args) {
	(void)args;
	exception_t *self = super_ctor(Exception(), _self, args);
	self->msg = va_arg(*args, char *);
	return self;
}

int exception_put(void *_self, FILE *stream, char *subformat) {
	(void)subformat;
	exception_t *self = cast(Exception(), _self);
	return file_print(stream, "%s: %s", class_get_name(class_of(self)), self->msg);
}

class_t *Exception() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), __FUNCTION__, Object(), sizeof(exception_t),
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
	stack_frame_t *self = super_ctor(StackFrame(), _self, args);
	self->thrown  = NULL;
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

stack_t *stack_trace = NULL;

jmp_buf *exception_push() {
	if (stack_trace == NULL)
		stack_trace = new(Stack());
	stack_frame_t *e = new(StackFrame());
	stack_push(stack_trace, e);
	return &e->buf;
}

void exception_throw(void *something) {
	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		fatalf("uncaught exception: %O", something);
	}
	else {
		stack_frame_t *e = stack_top(stack_trace);
		e->thrown  = something;
		longjmp(e->buf, 0);
	}
}

void exception_throw_uncaught() {
	if (!stack_is_empty(stack_trace)) {
		stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
		void *thrown = frame->thrown;
		delete(frame);
		if (thrown)
			exception_throw(thrown);
	}
}

void *exception_top() {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	return frame->thrown;
}

void *exception_catch() {
	stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
	void *thrown   = frame->thrown;
	delete(frame);
	return thrown;
}
