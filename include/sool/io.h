#ifndef __IO_H__
#define __IO_H__

#include <stdarg.h>
#include <stdio.h>
#include <sool/args.h>
#include <sool/exception.h>
#include <sool/list.h>

#define print(format, args...) file_print(stdout, format, ## args)

class_t *IOError();

FILE   *file_open      (const char *path, const char *mode);
void   *file_read      (FILE *self, size_t size);
void    file_write     (FILE *self, void *data, size_t size);
int     file_print     (FILE *self, const char *format, ...);
int     file_vprint    (FILE *self, const char *format, va_list args);
list_t *file_readlines (FILE *self);
void    file_close     (FILE *self);

#endif
