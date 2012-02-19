#ifndef __LIST_H__
#define __LIST_H__

#include <sool/object.h>
#include <sool/args.h>
#include <sool/sequence.h>
#include <stdbool.h>

// TODO lists are supposed to be mutable (as in python)
// --> need set,
// sort should be in place
// and join should be renamed extend and be in place

struct _list_t;
typedef struct _list_t list_t;

struct _list_iterator_t;
typedef struct _list_iterator_t list_iterator_t;

#define list(args...)       new(List(),__VA_N_ARGS__(args),  ##args)
#define list_join(args...)  _list_join(__VA_N_ARGS__(args),  ##args)

class_t *List();     /* new(List(), n, x1, x2, ..., xn) */

list_t     *list_clone        (list_t *self);
list_t     *list_copy         (list_t *self, iterator_t *from, iterator_t *to);
list_t     *list_slice        (list_t *self, int from, int to);
list_t    *_list_join         (int n, ...);
list_t     *list_sort         (list_t *self);
list_t     *list_reverse      (list_t *self);
bool        list_is_empty     (list_t *self);
int         list_count        (list_t *self, void *value);
int         list_length       (list_t *self);
void        list_clear        (list_t *self);
iterator_t *list_get          (list_t *self, int i);
iterator_t *list_find         (list_t *self, void *value);
iterator_t *list_rfind        (list_t *self, void *value);
iterator_t *list_insert_before(list_t *self, iterator_t *item, void *value);
iterator_t *list_insert_after (list_t *self, iterator_t *item, void *value);
iterator_t *list_prepend      (list_t *self, void *value);
iterator_t *list_append       (list_t *self, void *value);
void       *list_remove       (list_t *self, iterator_t *to_remove);
void       *list_remove_first (list_t *self);
void       *list_remove_last  (list_t *self);

#endif
