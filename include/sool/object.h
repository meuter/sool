#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdio.h>

#include <sool/args.h>
#include <stdbool.h>

struct _object_t;
struct _class_t;

typedef struct _object_t object_t;
typedef struct _class_t class_t;

class_t *Object(); /* new(Object()) */
class_t *Class();  /* new(Class(), "name", Super(), size, selector, method, ..., NULL); */

class_t *ClassCastError();
class_t *NullPointerError();

#define delete(args...) _delete(__VA_N_ARGS__(args), ##args)

void *new     (class_t *class, ...);
void _delete  (int n, ...);

void *ctor(void *self, va_list *args);
void *dtor(void *self);
int   put (void *self, FILE *stream, const char *format);
int   cmp (void *self, void *other);

bool     equal    (void *self, void *other);
void    *cast     (class_t *class, void *self);
bool     is_a(class_t *class, void *_self);
class_t *class_of (void *self);
class_t *super    (void *self);
size_t   size_of  (void *self);
bool     is_object(void *_self);

const char *class_get_name(void *_self);

#endif
