#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include <sool/object.h>

typedef void sequence_t;
typedef void iterator_t;

iterator_t *begin   (sequence_t *self);
iterator_t *end     (sequence_t *self);
iterator_t *rbegin  (sequence_t *self);
iterator_t *rend    (sequence_t *self);
iterator_t *find    (sequence_t *self, void *value);
iterator_t *rfind   (sequence_t *self, void *value);

iterator_t *next    (iterator_t *self);
iterator_t *previous(iterator_t *self);
void       *get     (iterator_t *self);

#define forall(i,s) for(i = begin(s); i != end(s); i = next(i))
#define rforall(i,s) for (i = rbegin(s); i != rend(s); i = previous(i))

#endif
