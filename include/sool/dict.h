#ifndef __DICT_H__
#define __DICT_H__

#include <sool/object.h>
#include <sool/args.h>

struct _dict_t;
typedef struct _dict_t dict_t;

typedef unsigned long(*hash_function_t)(void *key);

#define dict(n, args...) (new(Dictionary(), NULL, __VA_N_ARGS__(args), ## args))

class_t *Dictionary(); /* new(Dictionary(), hash_function, n, key1, value1, ..., key_n, value_n); */

dict_t *dict_clone (dict_t *self);
void   *dict_get   (dict_t *self, void *key);
void   *dict_put   (dict_t *self, void *key, void *value);
void    dict_clear (dict_t *self);

#endif
