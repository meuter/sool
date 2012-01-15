#ifndef __LIST_H__
#define __LIST_H__

#include <cobject/class.h>
#include <cobject/args.h>
#include <cobject/bool.h>

struct _list_t;
typedef struct _list_t list_t;

#define list(args...) new(List(), __VA_N_ARGS__(args), ##args);

const class_t *List();     /* new(List(), n, x1, x2, ..., xn) */

list_t *list_clone         (list_t *self);
list_t *list_slice         (list_t *self, int from, int to);
list_t *list_join          (list_t *self, ...);
list_t *list_sort          (list_t *self);
bool_t  list_is_empty      (list_t *self);
int     list_get_length    (list_t *self);
void   *list_get_first     (list_t *self);
void   *list_get_last      (list_t *self);
void   *list_get           (list_t *self, int i);
void    list_append        (list_t *self, void *element);
void    list_prepend       (list_t *self, void *element);
void   *list_remove_first  (list_t *self);
void   *list_remove_last   (list_t *self);

#endif
