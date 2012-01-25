#ifndef __OBJECT_DEF_H__
#define __OBJECT_DEF_H__

#include <sool/object.h>
#include <stdint.h>

#define MAGIC  0x0EFFACED

struct _object_t {
	uint32_t magic;
	class_t *class;
};

struct _class_t {
	const object_t object;
	size_t size;
	const char *name;
	class_t *super;
	void * (*ctor) (void *self, va_list *args);
	void * (*dtor) (void *self);
	int    (*put)  (void *self, FILE *stream, const char *format);
	int    (*cmp)  (void *self, void *other);
};

#endif
