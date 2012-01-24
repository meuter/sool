#ifndef __OBJECT_DEF_H__
#define __OBJECT_DEF_H__

#include <sool/object.h>

struct _object_t {
	class_t *class;
};

struct _class_t {
	const char _[sizeof(object_t)];
	size_t size;
	const char *name;
	class_t *super;
	void * (*ctor) (void *self, va_list *args);
	void * (*dtor) (void *self);
	int    (*put)  (void *self, FILE *stream, const char *format);
	int    (*cmp)  (void *self, void *other);
};

#endif
