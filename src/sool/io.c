#include <sool/object.h>
#include <sool/io.h>
#include <sool/bool.h>
#include <sool/string.h>
#include <sool/exception.h>
#include <string.h>
#include <assert.h>

#include "exception_def.h"

DEFINE_EXCEPTION(IOError);

/*****************************************************************************/

static bool_t is_format_character(char c) {
	switch(c) {
	case '\0':
	case 'd': case 'i':
	case 'o': case 'u': case 'x': case 'X':
	case 'e': case 'E':
	case 'f': case 'F':
	case 'g': case 'G':
	case 'a': case 'A':
	case 'c':
	case 's':
	case 'p':
	case 'n':
	case 'm':
	case '%':
		return TRUE;
	default:
		return FALSE;
	}
}

static int parse_format(const char *string) {
	int i = 0;

	while (string[i]) {
		if (is_format_character(string[i]))
			return i;
		i++;
	}

	return -1;
}

static int parse_curly_braces(const char *string) {
	int i = 0;
	int n_open = 0;

	if (string[0] != '{')
		return 0;

	while (string[i]) {
		if (string[i] == '{') {
			n_open++;
		}
		if (string[i] == '}') {
			n_open--;
			if (n_open == 0)
				return i;
		}
		++i;
	}
	return -1;
}

/*****************************************************************************/


#define TRACE(x) x

int ovfprintf(FILE *stream, const char *format, va_list args) {
	int result = 0;
	int i = 0;

	while ( format[i] ) {
		if (format[i] == '%') {
			if (format[i+1] == '@') {
				int j = parse_curly_braces(&format[i+2]);
				if (j > 0) {
					char *subformat = string_slice(format, i+3, i+j+2);
					result += put(va_arg(args, void *), stream, subformat);
					delete(subformat);
					i += 2+j+1;
				}
				else {
					result += put(va_arg(args, void *), stream, NULL);
					i += 2;
				}
			}
			else {
				int j = parse_format(&format[i+1]);
				char *subformat = string_slice(format, i, i+j+2);
				result += vfprintf(stream, subformat, args);
				delete(subformat);
				(void)va_arg(args, void *);
				i += j+2;
			}
		}
		else {
			putc(format[i], stream);
			result += 1;
			i += 1;
		}
	}
	return result;
}



























int oprintf(const char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	result = ovfprintf(stdout, format, args);
	va_end(args);
	return result;
}

int ovprintf(const char *format, va_list ap) {
	return ovfprintf(stdout, format, ap);
}

int ofprintf(FILE *stream, const char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	result = ovfprintf(stream, format, args);
	va_end(args);
	return result;
}
