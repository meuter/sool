#ifndef __STRING_H__
#define __STRING_H__

#include <cobject/class.h>
#include <cobject/args.h>
#include <cobject/list.h>
#include <string.h>

struct _string_t;
typedef struct _string_t string_t;

#define string(char_ptr)      new(String(), strlen(char_ptr), char_ptr)
#define string_join(args...)  _string_join(__VA_N_ARGS__(args, ##args))

const class_t *String();	   /* new(String(), 11, "hello world") */

string_t  *string_clone    (string_t *self);
string_t  *string_slice    (string_t *self, int from, int to);
string_t *_string_join     (int n, ...);
string_t  *string_join_list(list_t *list);
int        string_length   (string_t *self);
char      *string_cstr     (string_t *self);
list_t    *string_split    (string_t *self, const char *delimiter);
int       *string_find     (string_t *self, string_t *substr);
char       string_get      (string_t *self, int i);
char       string_set      (string_t *self, int i, char c);

#endif
