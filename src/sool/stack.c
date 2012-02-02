#include <sool/stack.h>
#include <sool/list.h>
#include <sool/error.h>

#include "object_def.h"

struct _stack_t {
	EXTENDS(object_t);
	list_t *list;
};

void *stack_ctor(void *_self, va_list *args) {
	(void)args;
	stack_t *self = cast(Stack(), _self);
	self->list = list();
	return self;
}

void *stack_dtor(void *_self) {
	stack_t *self = cast(Stack(), _self);
	delete(self->list);
	return self;
}

class_t *Stack() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), "Stack", Class(), sizeof(stack_t),
			ctor, stack_ctor,
			dtor, stack_dtor,
			NULL
		);
	}
	return result;
}

void stack_push(stack_t *self, void *info) {
	(void)list_prepend(self->list, info);
}

void *stack_pop(stack_t *self) {
	assertf(!stack_is_empty(self), "stack underflow");
	return list_remove_first(self->list);
}

void *stack_top(stack_t *self) {
	assertf(!stack_is_empty(self), "stack underflow");
	return get(begin(self->list));
}

bool_t stack_is_empty(stack_t *self) {
	return list_is_empty(self->list);
}

void stack_clear(stack_t *self) {
	list_clear(self->list);
}

