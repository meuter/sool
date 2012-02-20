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
	self->stage = __TRY;
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
	if (thrown) throw(thrown);
}

void frame_jmp(stack_frame_t *frame) {
	frame = cast(StackFrame(), frame);
	longjmp(frame->buf, 0);
}

void *frame_get_thrown(stack_frame_t *frame) {
	frame = cast(StackFrame(), frame);
	return frame->thrown;
}

void frame_set_stage(stack_frame_t *frame, frame_stage_t stage) {
	frame = cast(StackFrame(), frame);
	frame->stage = stage;
	if (stage == __FINALLY) frame->thrown = NULL;
}

frame_stage_t frame_get_stage(stack_frame_t *frame) {
	frame = cast(StackFrame(), frame);
	return frame->stage;
}

void frame_throw(void *something) {

	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		fatalf("uncaught exception: %O", something);
	}
	else {
		stack_frame_t *frame = frame_top();
		frame->thrown = something;
		frame_jmp(frame);
	}
}







/*****************************************************************************/


jmp_buf *exception_push() {
	if (stack_trace == NULL)
		stack_trace = new(Stack());
	stack_frame_t *frame = new(StackFrame());
	stack_push(stack_trace, frame);
	return &frame->buf;
}

void exception_throw(void *something) {

	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		fatalf("uncaught exception: %O", something);
	}
	else {
		stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
		frame->thrown = something;
		longjmp(frame->buf, 0);
	}
}

void exception_throw_uncaught() {
	// if top of the stack has something thrown, re-throw it
	if (stack_trace && !stack_is_empty(stack_trace)) {
		stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
		if (frame->thrown) {
			void *thrown = frame->thrown;
			(void)stack_pop(stack_trace);
			delete(frame);
			if (thrown) exception_throw(thrown);
		}
	}
}

void *exception_top() {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	return frame->thrown;
}

void *exception_pop() {
	stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
	void *thrown = frame->thrown;
	delete(frame);
	return thrown;
}
