#include <sool/error.h>
#include <sool/io.h>
#include <signal.h>

#define stack_t sool_stack_t
#include <sool/stack.h>

#include "exception_def.h"

DEFINE_EXCEPTION(SegmentationFaultError);

//#define EXCEPTION_THROWN   2
#define EXCEPTION_FINISHED 3


/*****************************************************************************/

class_t *StackFrame();

typedef struct {
	EXTENDS(object_t);
	jmp_buf buf;
	void *thrown;
	int stage;
} stack_frame_t;


void *stack_frame_ctor(void *_self, va_list *args) {
	(void)args;
	stack_frame_t *self = super_ctor(StackFrame(), _self, args);
	self->thrown = NULL;
	self->stage = 0;
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

__thread stack_t *stack_trace = NULL;

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

static void exception_handle_signal(int signum) {
	switch(signum) {
	case SIGSEGV:
		throw(new(SegmentationFaultError(), "SIGSEGV recieved"));
		break;
	}
}

jmp_buf *exception_push() {
	if (stack_trace == NULL) {
		stack_trace = new(Stack());
		signal(SIGSEGV, exception_handle_signal);
	}
	stack_frame_t *frame = new(StackFrame());
	stack_push(stack_trace, frame);
	return &frame->buf;
}

void exception_pop() {
	stack_frame_t *frame = cast(StackFrame(), stack_pop(stack_trace));
	int stage = frame->stage;
	void *thrown = frame->thrown;
	delete(frame);
	if (thrown && stage == 2) throw(thrown);
}

void exception_jmp(int stage) {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	frame->stage = stage;
	longjmp(frame->buf, 0);
}

void *exception_get() {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	return frame->thrown;
}

int exception_step() {
	stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
	return frame->stage;
}


void exception_throw(void *something) {
	while (stack_trace != NULL && !stack_is_empty(stack_trace) && exception_step() == 2) {
			delete(stack_pop(stack_trace));
	}

	if (stack_trace == NULL || stack_is_empty(stack_trace)) {
		fatalf("uncaught exception: %O", something);
	}
	else {
		stack_frame_t *frame = cast(StackFrame(), stack_top(stack_trace));
		frame->thrown = something;
		exception_jmp(2);
	}
}
