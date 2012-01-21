#ifndef __IO_H__
#define __IO_H__

#include <stdarg.h>
#include <stdio.h>

int oprintf(const char *format, ...);
int ofprintf(FILE *stream, const char *format, ...);
int ovprintf(const char *format, va_list ap);
int ovfprintf(FILE *stream, const char *format, va_list ap);

// TODO add osprintf osnprintf ovsprintf ovsnprintf

#endif
