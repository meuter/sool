#ifndef __LIST_H__
#define __LIST_H__

#include <cobject/class.h>
#include <cobject/args.h>
#include <cobject/bool.h>

struct _list_t;
typedef struct _list_t *list_t;

struct _item_t;
typedef struct _item_t *item_t;

#define list(args...)       new(List(),__VA_N_ARGS__(args),  ##args)
#define list_join(args...)  _list_join(__VA_N_ARGS__(args),  ##args)
#define list_forall(i,l)    for (i = list_begin(l); i != list_end(l); i = item_next(i))

const class_t *List();     /* new(List(), n, x1, x2, ..., xn) */

list_t  list_clone        (list_t self);
list_t  list_copy         (list_t self, item_t from, item_t to);
list_t  list_slice        (list_t self, int from, int to);
list_t _list_join         (int n, ...);
list_t  list_sort         (list_t self);
list_t  list_reverse      (list_t self);
bool_t  list_is_empty     (list_t self);
int     list_count        (list_t self, void *value);
int     list_length       (list_t self);
void    list_clear        (list_t self);
item_t  list_get          (list_t self, int i);
void    list_set          (list_t self, int i, void *value);
item_t  list_find         (list_t self, void *value);
item_t  list_rfind        (list_t self, void *value);
item_t  list_prepend      (list_t self, void *value);
item_t  list_append       (list_t self, void *value);
item_t  list_begin        (list_t self);
item_t  list_end          (list_t self);
item_t  list_rbegin       (list_t self);
item_t  list_rend         (list_t self);
void   *list_remove       (list_t self, item_t to_remove);
void   *list_remove_first (list_t self);
void   *list_remove_last  (list_t self);
void   *item_get          (item_t self);
void    item_set          (item_t self, void *value);
item_t  item_next         (item_t self);
item_t  item_previous     (item_t self);

#endif
