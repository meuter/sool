#include <sool/object.h>
#include <sool/error.h>
#include <sool/mem.h>
#include <assert.h>
#include <string.h>


#include "object_def.h"

extern class_t _Object;
extern class_t _Class;

// TODO write unit test for object module

/*****************************************************************************/

void *object_ctor(void *self, va_list *args) {
	(void)args;
	return self;
}

void *object_dtor(void *self) {
	return self;
}

int object_put(void *self, FILE *stream, const char *format) {
	(void)format;
	class_t *class = class_of(self);
	return fprintf(stream, "<instance of %s at address %p>", class->name, self);
}

int object_cmp(void *self, void *other) {
	return (int)self-(int)other;
}

class_t _Object = {
	.parent = {
		.magic = MAGIC,
		.class = &_Class,
	},
	.size  = sizeof(object_t),
	.name  = "Object",
	.super = &_Object,
	.ctor  = object_ctor,
	.dtor  = object_dtor,
	.put   = object_put,
	.cmp   = object_cmp
};

class_t *Object() {
	return &_Object;
}

/*****************************************************************************/

void *class_ctor(void *_self, va_list *args) {
	class_t *self = cast(Class(), _self);
	const size_t offset = offsetof(class_t, ctor);

	self->name  = va_arg(*args, char *);
	self->super = va_arg(*args, class_t *);
	self->size  = va_arg(*args, size_t);

	assert(self->super);

	memcpy((char*)self + offset, (char *)self->super + offset, size_of(self->super) - offset);

	{
		typedef void (*method_t)();

		method_t selector;

		va_list args_copy = *args;

		while ( (selector = va_arg(args_copy, method_t)) ) {
			if (selector == (method_t)ctor)
				*(method_t*) &self->ctor = va_arg(args_copy, method_t);
			if (selector == (method_t)dtor)
				*(method_t*) &self->dtor = va_arg(args_copy, method_t);
			if (selector == (method_t)put)
				*(method_t*) &self->put = va_arg(args_copy, method_t);
			if (selector == (method_t)cmp)
				*(method_t*) &self->cmp = va_arg(args_copy, method_t);
		}
	}

	return self;
}

void *class_dtor(void *self) {
	(void)self;
	return NULL;
}


class_t _Class = {
	.parent = {
		.magic = MAGIC,
		.class = &_Class,
	},
	.size  = sizeof(class_t),
	.name  = "Class",
	.super = &_Object,
	.ctor  = class_ctor,
	.dtor  = class_dtor,
	.put   = object_put,
	.cmp   = object_cmp
};

class_t *Class() {
	return &_Class;
}

/*****************************************************************************/

void *new(class_t *class, ...) {
	object_t *object = xmalloc(class->size);
	object->magic = MAGIC;
	object->class = class;
	va_list ap;
	va_start(ap, class);
	ctor(object, &ap);
	va_end(ap);

	return object;
}

void _delete  (int n, ...) {
	va_list args;

	va_start(args, n);
	while (n--)
		xfree(dtor(va_arg(args, object_t *)));
	va_end(args);
}

void *ctor(void *self, va_list *args) {
	class_t *class = class_of(self);
	assert(class->ctor);
	return class->ctor(self, args);
}

void *dtor(void *self) {
	class_t *class = class_of(self);
	assert(class->dtor);
	return class->dtor(self);
}

int put(void *self, FILE *stream, const char *format) {
	class_t *class = class_of(self);
	assert(class->put);
	return class->put(self, stream, format);
}

int cmp(void *self, void *other) {
	class_t *class = class_of(self);
	assert(class->cmp);
	return class->cmp(self, other);
}

bool_t equal   (void *self, void *other) {
	return cmp(self, other) == 0;
}

void *cast(class_t *class, void *_self) {
	object_t *self = _self;
	assert(is_object(self));
	class_t *current = self->class, *parent;

	while (current && current != class) {
		 parent = super(current);
		 assertf(parent != current, "could not cast '%O' to class '%s'", self, class->name);
		 current = parent;
	}
	return self;
}

class_t *class_of(void *_self) {
	object_t *self = cast(Object(), _self);
	return self->class;
}

size_t size_of(void *_self) {
	class_t *class = class_of(_self);
	assert(class);
	return class->size;
}

bool_t is_object(void *_self) {
	object_t *self = _self;
	return (self && self->magic == MAGIC);
}

class_t *super(void *_self) {
	class_t *self = cast(Class(), _self);
	assert(self->super);
	return self->super;
}

void *super_ctor(void *class, void *self, va_list *args) {
	class_t *super_class = cast(Class(), super(class));
	assert(super_class->ctor);
	return super_class->ctor(self, args);
}

void *super_dtor(void *class, void *self) {
	class_t *super_class = cast(Class(), super(class));
	assert(super_class->dtor);
	return super_class->dtor(self);
}

