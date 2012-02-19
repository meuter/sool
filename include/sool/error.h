#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdarg.h>
#include <stdio.h>

void fatalf(char *error_message, ...);
void error_print_backtrace(FILE *stream);

#endif
