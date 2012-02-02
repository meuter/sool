#ifndef __STACK_H__
#define __STACK_H__

#include <sool/object.h>

struct _stack_t;
typedef struct _stack_t stack_t;

class_t *Stack(); /* new(Stack()) */

void   stack_push    (stack_t *self, void *info);
void  *stack_pop     (stack_t *self);
void  *stack_top     (stack_t *self);
bool_t stack_is_empty(stack_t *self);
void   stack_clear   (stack_t *self);

#endif
