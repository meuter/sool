#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdarg.h>

#define assertf(expr, msg, args...) _assertf((int)(expr), msg, ## args)

void _assertf(int expression, char *error_message, ...);

#endif
