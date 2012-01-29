#ifndef __OBJECT_DEF_H__
#define __OBJECT_DEF_H__

#include <sool/object.h>
#include <stdint.h>

#define MAGIC  			(void *)0x0EFFACED
#define EXTENDS(type)   const type parent

struct _object_t {
	void *magic;
	class_t *class;
};

struct _class_t {
	EXTENDS(object_t);
	size_t size;
	const char *name;
	class_t *super;
	void * (*ctor) (void *self, va_list *args);
	void * (*dtor) (void *self);
	int    (*put)  (void *self, FILE *stream, const char *format);
	int    (*cmp)  (void *self, void *other);
};

void *super_ctor(void *class, void *self, va_list *args);
void *super_dtor(void *class, void *self);

#endif
