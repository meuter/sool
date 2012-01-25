#include "sequence_def.h"
#include <assert.h>

/*****************************************************************************/

void *iterator_class_ctor(void *self, va_list *args) {
	(void)self;
	(void)args;
	// TODO fill function
	return NULL;
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

void *sequence_class_ctor(void *self, va_list *args) {
	(void)self;
	(void)args;
	// TODO fill function
	return NULL;
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

