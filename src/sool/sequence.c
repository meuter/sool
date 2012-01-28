#include "sequence_def.h"
#include <assert.h>

/*****************************************************************************/

void *iterator_class_ctor(void *_self, va_list *args) {
	iterator_class_t *self = super_ctor(IteratorClass(), _self, args);
	typedef void (*method_t)();
	method_t selector;

	self->next = self->get = NULL;

	va_list args_copy = *args;
	while ( (selector = va_arg(args_copy, method_t)) ) {
		if (selector == (method_t)next)
			*(method_t *) &self->next = va_arg(args_copy, method_t);
		if (selector == (method_t)get)
			*(method_t *) &self->get = va_arg(args_copy, method_t);
	}

	return self;
}

class_t *IteratorClass() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), __FUNCTION__, Class(), sizeof(iterator_class_t),
			ctor, iterator_class_ctor,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

void *sequence_class_ctor(void *_self, va_list *args) {
	sequence_class_t *self = super_ctor(Class(), _self, args);
	typedef void (*method_t)();
	method_t selector;

	self->begin = self->end = NULL;

	va_list args_copy = *args;
	while ( (selector = va_arg(args_copy, method_t)) ) {
		if (selector == (method_t)begin)
			*(method_t *) &self->begin = va_arg(args_copy, method_t);
		if (selector == (method_t)end)
			*(method_t *) &self->end = va_arg(args_copy, method_t);
	}

	return self;
}

class_t *SequenceClass() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), __FUNCTION__, Class(), sizeof(sequence_class_t),
			ctor, sequence_class_ctor,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

void *begin(void *self) {
	sequence_class_t *class = cast(SequenceClass(), class_of(self));
	assert(class->begin);
	return class->begin(self);
}

void *end  (void *self) {
	sequence_class_t *sequence = cast(SequenceClass(), class_of(self));
	assert(sequence->end);
	return sequence->end(self);
}

void *next(void *self) {
	iterator_class_t *class = cast(IteratorClass(), class_of(self));
	assert(class->next);
	return class->next(self);
}

void *get(void *self) {
	iterator_class_t *class = cast(IteratorClass(), class_of(self));
	assert(class->get);
	return class->get(self);
}

