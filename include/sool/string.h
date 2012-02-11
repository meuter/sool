#ifndef __STRING_H__
#define __STRING_H__

#include <sool/args.h>
#include <sool/list.h>
#include <string.h>

#define string_join(self, args...)  _string_join(self, __VA_N_ARGS__(args), ##args)

char   *string_clone       (const char *self);
size_t  string_length      (const char *self);
char    string_get         (const char *self, int i);
int     string_find        (const char *self, const char *substr);
int     string_rfind       (const char *self, const char *substr);
bool_t  string_contains    (const char *self, const char *substr);
char   *string_strip       (const char *self);
list_t *string_split       (const char *self, const char *delimiter);
char  *_string_join        (const char *self, int n, ...);
char   *string_slice       (const char *self, int from, int to);
char   *string_replace     (const char *self, const char *substr, const char *other);
int     string_cmp         (const char *self, const char *other);
bool_t  string_equal       (const char *self, const char *other);
char   *string_ljust       (const char *self, int width, char filler);
char   *string_rjust       (const char *self, int width, char filler);
char   *string_center      (const char *self, int width, char filler);
char   *string_lower       (const char *self);
char   *string_upper       (const char *self);
char   *string_title       (const char *self);
bool_t  string_starts_with (const char *self, const char *substr);
bool_t  string_ends_with   (const char *self, const char *substr);
char   *string_format      (const char *self, ...);
bool_t  string_is_space    (const char *self);
bool_t  string_is_lower    (const char *self);
bool_t  string_is_upper    (const char *self);
bool_t  string_is_alpha    (const char *self);
bool_t  string_is_num      (const char *self);
bool_t  string_is_alphanum (const char *self);



#endif
