#include <cobject/class.h>
#include <malloc.h>

struct _class_t {
	size_t size;
	const char *name;
	ctor_t ctor;
	dtor_t dtor;
	put_t put;
	cmp_t cmp;
};

typedef struct {
	const class_t *class;
} object_t;

class_t *class(const size_t size, const char *name, ctor_t ctor, dtor_t dtor, put_t put, cmp_t cmp) {
	class_t *class = malloc(sizeof(class_t));
	class->size = size;
	class->name = name;
	class->ctor = ctor;
	class->dtor = dtor;
	class->put  = put;
	class->cmp  = cmp;
	return class;
}

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

void delete(void *self) {
	object_t *object = self;

	if (object->class->dtor)
		object->class->dtor(self);

	free(self);
}

int fputof(void *_self, FILE *stream, const char *format) {
	object_t *self = _self;

	if (self->class->put)
		return self->class->put(self, stream, format);
	else
		return fprintf(stream, "<%s object at %p>", self->class->name, self);
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

class_t *class_of(void *_self) {
	object_t *self = _self;
	return self->class;
}

size_t  *size_of(void *_self) {
	object_t *self = _self;
	self->class->size;
}
