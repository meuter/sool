#include <sool/object.h>
#include <sool/error.h>
#include <sool/mem.h>
#include <sool/exception.h>
#include <string.h>

#include "object_def.h"
#include "exception_def.h"

extern class_t _Object;
extern class_t _Class;

DEFINE_EXCEPTION(ClassCastError);

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

typedef void (*method_t)();

void *class_ctor(void *_self, va_list *args) {
	class_t *self = cast(Class(), _self);
	const size_t offset = offsetof(class_t, ctor);
	method_t selector;
	va_list args_copy;

	self->name  = va_arg(*args, char *);
	self->super = va_arg(*args, class_t *);
	self->size  = va_arg(*args, size_t);

	if (self->super == NULL) throw(new(NullPointerError()));
	if (self->name == NULL)  throw(new(NullPointerError()));

	// copy methods of super class
	memcpy((char*)self + offset, (char *)self->super + offset, size_of(self->super) - offset);

	// override method from argument list
	args_copy = *args;
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

	return self;
}

void *class_dtor(void *self) {
	(void)self;
	return NULL;
}


int class_put(void *_self, FILE *stream, const char *format) {
	(void)format;
	class_t *class = cast(Class(), _self);
	return fprintf(stream, "%s", class->name);
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
	object_t *object = mem_alloc(class->size);
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
	while (n--) {
		void *ptr = va_arg(args, void *);
		if (is_object(ptr))
			mem_free(dtor(ptr));
		else
			mem_free(ptr);
	}
	va_end(args);
}

void *ctor(void *self, va_list *args) {
	class_t *class = class_of(self);
	if (class->ctor == NULL) throw(new(NullPointerError()));
	return class->ctor(self, args);
}

void *dtor(void *self) {
	class_t *class = class_of(self);
	if (class->dtor == NULL) throw(new(NullPointerError()));
	return class->dtor(self);
}

int put(void *self, FILE *stream, const char *format) {
	class_t *class = class_of(self);
	if (class->put == NULL) throw(new(NullPointerError()));
	return class->put(self, stream, format);
}

int cmp(void *self, void *other) {
	class_t *class = class_of(self);
	if (class->cmp == NULL) throw(new(NullPointerError()));
	return class->cmp(self, other);
}

bool_t equal   (void *self, void *other) {
	return cmp(self, other) == 0;
}

void *cast(class_t *class, void *_self) {
	object_t *self = _self;
	if (self == NULL) 			throw(new(NullPointerError()));
	if (!is_object(self))		throw(new(ClassCastError()));

	class_t *current = self->class, *parent;
	if (self->class == NULL) 	throw(new(NullPointerError()));

	while (current && current != class) {
		 parent = super(current);
		 if (current == parent)	throw(new(ClassCastError()));
		 current = parent;
	}
	return self;
}

bool_t is_a(class_t *class, void *_self) {
	bool_t result = FALSE;
	exception_t *e;
	try {
		cast(class, _self);
		return TRUE;
	}
	catch(ClassCastError(), e) {
		delete(e);
	}
	return result;
}


class_t *class_of(void *_self) {
	object_t *self = cast(Object(), _self);
	return self->class;
}

const char *class_get_name(void *_self) {
	class_t *self = cast(Class(), _self);
	return self->name;
}

size_t size_of(void *_self) {
	class_t *class = class_of(_self);
	return class->size;
}

bool_t is_object(void *_self) {
	object_t *self = _self;
	return (self && self->magic == MAGIC);
}

class_t *super(void *_self) {
	class_t *self = cast(Class(), _self);
	return self->super;
}

void *super_ctor(void *class, void *self, va_list *args) {
	class_t *super_class = cast(Class(), super(class));
	if (super_class->ctor == NULL) throw(new(NullPointerError()));
	return super_class->ctor(self, args);
}

void *super_dtor(void *class, void *self) {
	class_t *super_class = cast(Class(), super(class));
	if (super_class->dtor == NULL) throw(new(NullPointerError()));
	return super_class->dtor(self);
}
