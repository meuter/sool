#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

#include <sool/object.h>
#include <sool/args.h>

extern const int STRING_BUFFER_DEFAULT_SIZE;

struct _string_buffer;
typedef struct _string_buffer string_buffer_t;

class_t *StringBuffer(); /* new(StringBuffer()) */

int     string_buffer_append (string_buffer_t *self, const char *format, ...);
int     string_buffer_vappend(string_buffer_t *self, const char *format, va_list args);
size_t  string_buffer_length (string_buffer_t *self);
size_t  string_buffer_size   (string_buffer_t *self);
char   *string_buffer_value  (string_buffer_t *self);
void    string_buffer_reset  (string_buffer_t *self);
void    string_buffer_detach (string_buffer_t *self);


#endif
