#include <sool/class.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

// TODO write unit tests for the class module
struct _class_t {
	size_t size;
	const char *name;
	class_t *super;
	ctor_t ctor;
	dtor_t dtor;
	put_t  put;  // TODO split put into str and repr ?
	cmp_t  cmp;
	// TODO add clone method
	// TODO add a hash method for dict
};

typedef struct {
	const class_t *class;
} object_t;


void ctor(void *_self, va_list *args) {
	object_t *self = _self;
	if (self->class && self->class->ctor)
		self->class->ctor(self, args);
}

void dtor(void *_self) {
	object_t *self = _self;
	if (self->class && self->class->dtor)
		self->class->dtor(self);
}

int put(void *self, FILE *stream, const char *format) {
	return fputof(self, stream, format);
}


void class_ctor(void *_self, va_list *args) {
	class_t *self = _self;
	typedef void (* fptr_t) ();
	fptr_t selector;

	self->name  = va_arg(*args, char *);
	self->super = va_arg(*args, class_t *);
	self->size  = va_arg(*args, size_t);
	self->ctor  = self->super->ctor;
	self->dtor  = self->super->dtor;
	self->put   = self->super->put;
	self->cmp   = self->super->cmp;

	while ( (selector = va_arg(*args, fptr_t)) ) {
		if (selector == (fptr_t)ctor)
			self->ctor = va_arg(*args, ctor_t);
		else if (selector == (fptr_t)dtor)
			self->dtor = va_arg(*args, dtor_t);
		else if (selector == (fptr_t)put)
			self->put = va_arg(*args, put_t);
		else if (selector == (fptr_t)cmp)
			self->cmp = va_arg(*args, cmp_t);
	}
}

class_t *Class() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = malloc(sizeof(class_t));
		result->size = sizeof(class_t);
		result->name = "Class";
		result->ctor = class_ctor;
		result->dtor = NULL;
		result->put  = NULL;
		result->cmp  = NULL;
	}
	return result;
}


// TODO try to find a way to enfore type checking on variable arg constructors
void *new(const class_t *class, ...) {
	object_t *object;

	if ( (object = malloc(class->size)) == NULL )
		return NULL;

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
		object_t *object = va_arg(args, object_t *);
		dtor(object);
		free(object);
	}
	va_end(args);
}

int fputof(void *_self, FILE *stream, const char *format) {
	object_t *self = _self;

	if (self->class->put)
		return self->class->put(self, stream, format);
	else
		return fprintf(stream, "<%s object at %p>", self->class->name, _self);
}

int putof(void *_self, const char *format) {
	return fputof(_self, stdout, format);
}

int puto(void *_self) {
	return putof(_self, NULL);
}

int fputo(void *_self, FILE *stream) {
	return fputof(_self, stream, NULL);
}

int cmp(void *_self, void *_other) {
	object_t *self = _self, *other = _other;

	if (self->class != other->class)
		return FALSE;

	if (self->class->cmp)
		return self->class->cmp(_self, _other);
	else
		return ((int)_self - (int)_other);
}

bool_t equal(void *_self, void *_other) {
	return (cmp(_self, _other) == 0);
}


const class_t *class_of(void *_self) {
	object_t *self = _self;
	return self->class;
}

size_t size_of(void *_self) {
	object_t *self = _self;
	return self->class->size;
}
