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
char   *file_read      (FILE *self, size_t size);
void    file_write     (FILE *self, const char *data, size_t size);
int     file_print     (FILE *self, const char *format, ...);
list_t *file_readlines (FILE *self);
void    file_close     (FILE *self);






int oprintf(const char *format, ...);
int ofprintf(FILE *stream, const char *format, ...);
int osprintf(char *str, const char *format, ...);
int ossnprintf(char *str, size_t size, const char *format, ...);

int ovprintf(const char *format, va_list ap);
int ovfprintf(FILE *stream, const char *format, va_list ap);
int ovsprintf(char *str, const char *format, va_list ap);
int ovsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
