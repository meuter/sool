#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include <sool/object.h>

struct _iterator_class_t;
typedef struct _iterator_class_t iterator_class_t;

struct _sequence_class_t;
typedef struct _sequence_class_t sequence_class_t;

typedef object_t iterator_t;

class_t *SequenceClass();
class_t *IteratorClass();

void *begin(void *self);
void *end  (void *self);
void *next (void *self);
void *get  (void *self);

#define forall(i,s) for(i = begin(s); i != end(s); i = next(i))

#endif
