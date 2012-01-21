#include <sool/class.h>
#include <malloc.h>
#include <stdarg.h>

// TODO write unit tests for the class module

struct _class_t {
	// TODO add support for super class
	size_t size;
	const char *name;
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

class_t *class(const size_t size, const char *name, ctor_t ctor, dtor_t dtor, put_t put, cmp_t cmp) {
	class_t *class = malloc(sizeof(class_t));
	class->size  = size;
	class->name  = name;
	class->ctor  = ctor;
	class->dtor  = dtor;
	class->put   = put;
	class->cmp = cmp;
	return class;
}

// TODO try to find a way to enfore type checking on variable arg constructors
void *new(const class_t *class, ...) {
	object_t *object;

	if ( (object = malloc(class->size)) == NULL )
		return NULL;

	object->class = class;

	if (class->ctor) {
		va_list ap;
		va_start(ap, class);
		class->ctor(object, &ap);
		va_end(ap);
	}

	return object;
}

void _delete  (int n, ...) {
	va_list args;

	va_start(args, n);
	while (n--) {
		object_t *object = va_arg(args, object_t *);
		if (object->class->dtor)
			object->class->dtor(object);
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
