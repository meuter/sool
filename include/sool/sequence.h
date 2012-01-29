#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include <sool/object.h>

typedef void sequence_t;
typedef void iterator_t;

iterator_t *begin(sequence_t *self);
iterator_t *end  (sequence_t *self);
iterator_t *next (iterator_t *self);
void       *get  (iterator_t *self);

#define forall(i,s) for(i = begin(s); i != end(s); i = next(i))

#endif
