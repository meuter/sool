#ifndef __STRING_H__
#define __STRING_H__

#include <sool/args.h>
#include <sool/list.h>
#include <string.h>

#define string_join(self, args...)  _string_join(self, __VA_N_ARGS__(args, ##args))

size_t  string_length    (const char *self);
char   *string_clone     (const char *self);
char    string_get       (const char *self, int i);

char    string_set       (char *self, int i, char c);
int     string_find      (const char *self, const char *substr);
int     string_rfind     (const char *self, const char *substr);
bool_t  string_contains  (const char *self, const char *substr);
char   *string_strip     (const char *self);
list_t *string_split     (const char *self, const char *delimiter);
char  *_string_join      (const char *self, int n, ...);
char   *string_join_list (const char *self, list_t *list);
char   *string_slice     (const char *self, int from, int to);
int     string_cmp       (const char *self, const char *other);
bool_t  string_equal     (const char *self, const char *other);

#endif
