#include <sool/stack.h>
#include <sool/error.h>
#include <sool/io.h>

#include "exception_def.h"

stack_t *stack_trace = NULL;


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

struct _stack_frame_t  {
	EXTENDS(object_t);
	jmp_buf buf;
	void *thrown;
	frame_stage_t stage;
};


void *stack_frame_ctor(void *_self, va_list *args) {
	(void)args;
	stack_frame_t *self = super_ctor(StackFrame(), _self, args);
	self->thrown = NULL;
	self->stage = __ARMED;
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


jmp_buf *frame_push() {
	if (stack_trace == NULL)
		stack_trace = new(Stack());
	stack_frame_t *frame = new(StackFrame());
	stack_push(stack_trace, frame);
	return &frame->buf;
}

stack_frame_t *frame_top() {
	return cast(StackFrame(), stack_top(stack_trace));
}

void frame_pop() {
	stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
	frame_stage_t stage = frame->stage;
	void *thrown = frame->thrown;
	delete(frame);
	if (thrown && stage == __THROWN) throw(thrown);
}

void frame_jmp(frame_stage_t stage) {
	stack_frame_t *frame = frame_top();
	frame->stage = stage;
	longjmp(frame->buf, 0);
}

void *frame_thrown() {
	stack_frame_t *frame = frame_top();
	return frame->thrown;
}

frame_stage_t frame_stage() {
	stack_frame_t *frame = frame_top();
	return frame->stage;
}


void frame_throw(void *something) {
	while (stack_trace != NULL && !stack_is_empty(stack_trace) && frame_stage() == __THROWN) {
			delete(stack_pop(stack_trace));
	}

	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		fatalf("uncaught exception: %O", something);
	}
	else {
		stack_frame_t *frame = frame_top();
		frame->thrown = something;
		frame_jmp(__THROWN);
	}
}
