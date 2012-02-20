#include <sool/stack.h>
#include <sool/list.h>
#include <sool/error.h>

#include "object_def.h"
#include "exception_def.h"

DEFINE_EXCEPTION(StackUnderflowError);

/*****************************************************************************/

struct _stack_t {
	EXTENDS(object_t);
	list_t *list;
};

static void *stack_ctor(void *_self, va_list *args) {
	(void)args;
	stack_t *self = super_ctor(Stack(), _self, args);
	self->list = list();
	return self;
}

static void *stack_dtor(void *_self) {
	stack_t *self = super_dtor(Stack(), _self);
	delete(self->list);
	return self;
}

class_t *Stack() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), "Stack", Object(), sizeof(stack_t),
			ctor, stack_ctor,
			dtor, stack_dtor,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

void stack_push(stack_t *self, void *info) {
	(void)list_prepend(self->list, info);
}

void *stack_pop(stack_t *self) {
	if (stack_is_empty(self)) throw(new(StackUnderflowError(),""));
	return list_remove_first(self->list);
}

void *stack_top(stack_t *self) {
	if (stack_is_empty(self)) throw(new(StackUnderflowError(),""));
	return get(begin(self->list));
}

bool stack_is_empty(stack_t *self) {
	return list_is_empty(self->list);
}

void stack_clear(stack_t *self) {
	list_clear(self->list);
}

int stack_size(stack_t *self) {
	return list_length(self->list);
}
