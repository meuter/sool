#include "sequence_def.h"
#include <sool/error.h>

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
	sequence_class_t *self = super_ctor(SequenceClass(), _self, args);
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



iterator_t *begin(sequence_t *self) {
	sequence_class_t *class = cast(SequenceClass(), class_of(self));
	assertf(class->begin, "class '%O' has no 'begin' method", class);
	return class->begin(self);
}

iterator_t *end(sequence_t *self) {
	sequence_class_t *class = cast(SequenceClass(), class_of(self));
	assertf(class->end, "class '%O' has no 'end' method", class);
	return class->end(self);
}



iterator_t *next(iterator_t *self) {
	iterator_class_t *class = cast(IteratorClass(), class_of(self));
	assertf(class->next, "class '%O' has no 'next' method", class);
	return class->next(self);
}

void *get(iterator_t *self) {
	iterator_class_t *class = cast(IteratorClass(), class_of(self));
	assertf(class->get, "class '%O' has no 'get' method", class);
	return class->get(self);
}

