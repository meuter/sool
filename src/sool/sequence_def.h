#ifndef __SEQUENCE_DEF_H__
#define __SEQUENCE_DEF_H__

#include <sool/sequence.h>

#include "object_def.h"

struct _sequence_class_t {
	EXTENDS(class_t);
	void * (*begin) (void *self);
	void * (*end)   (void *self);
};

struct _iterator_class_t {
	EXTENDS(class_t);
	void * (*next) (void *self);
	void * (*get)  (void *self);
};

#endif
