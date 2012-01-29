#ifndef __SEQUENCE_DEF_H__
#define __SEQUENCE_DEF_H__

#include <sool/sequence.h>

#include "object_def.h"

typedef struct {
	EXTENDS(class_t);
	void * (*begin) (sequence_t *self);
	void * (*end)   (sequence_t *self);
} sequence_class_t;

typedef struct {
	EXTENDS(class_t);
	void * (*next) (iterator_t *self);
	void * (*get)  (iterator_t *self);
} iterator_class_t;

class_t *IteratorClass();
class_t *SequenceClass();

#endif
