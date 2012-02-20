#ifndef __STACK_H__
#define __STACK_H__

#include <sool/object.h>
#include <stdbool.h>

struct _stack_t;
typedef struct _stack_t stack_t;

class_t *Stack(); 					/* new(Stack()) */
class_t *StackUnderflowError();		/* new(StackUnderflowError()) */

void   stack_push    (stack_t *self, void *info);
void  *stack_pop     (stack_t *self);
void  *stack_top     (stack_t *self);
bool   stack_is_empty(stack_t *self);
void   stack_clear   (stack_t *self);
int    stack_size    (stack_t *self);

#endif
