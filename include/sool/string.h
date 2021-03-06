#ifndef __STRING_H__
#define __STRING_H__

#include <sool/args.h>
#include <sool/list.h>
#include <string.h>
#include <stdbool.h>

class_t *IndexError();

char   *string_clone       (const char *self);
size_t  string_length      (const char *self);
char    string_get         (const char *self, int i);
int     string_find        (const char *self, const char *substr);
int     string_rfind       (const char *self, const char *substr);
bool    string_contains    (const char *self, const char *substr);
char   *string_strip       (const char *self);
list_t *string_split       (const char *self, const char *delimiter);
char   *string_join        (const char *self, list_t *words, const char *format);
char   *string_slice       (const char *self, int from, int to);
char   *string_replace     (const char *self, const char *substr, const char *other);
int     string_cmp         (const char *self, const char *other);
bool    string_equal       (const char *self, const char *other);
char   *string_ljust       (const char *self, int width, char filler);
char   *string_rjust       (const char *self, int width, char filler);
char   *string_center      (const char *self, int width, char filler);
char   *string_lower       (const char *self);
char   *string_upper       (const char *self);
bool    string_starts_with (const char *self, const char *substr);
bool    string_ends_with   (const char *self, const char *substr);
char   *string_format      (const char *self, ...);
bool    string_is_space    (const char *self);
bool    string_is_lower    (const char *self);
bool    string_is_upper    (const char *self);
bool    string_is_alpha    (const char *self);
bool    string_is_num      (const char *self);
bool    string_is_alphanum (const char *self);

#endif
