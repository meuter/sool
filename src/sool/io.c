#include <sool/object.h>
#include <sool/io.h>
#include <sool/string.h>
#include <sool/exception.h>
#include <sool/mem.h>

#include <string.h>
#include <printf.h>
#include <errno.h>
#include <stdbool.h>

#include "exception_def.h"

DEFINE_EXCEPTION(IOError);

/*****************************************************************************/

static bool is_format_character(char c) {
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
		return true;
	default:
		return false;
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

FILE *file_open(const char *path, const char *mode) {
	FILE *result = fopen(path, mode);
	if (result == NULL)	throw(new(IOError(), strerror(errno)));
	return result;
}

void *file_read(FILE *self, size_t size) {
	void *result = mem_alloc(size);
	if (fread(result, size, 1, self) != size)
		throw(new(IOError(), strerror(errno)));
	return result;
}

void file_write(FILE *self, void *data, size_t size) {
	if (fwrite(data, size, 1, self))
		throw(new(IOError(), strerror(errno)));
}

int file_print(FILE *self, const char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	result = file_vprint(self, format, args);
	va_end(args);
	return result;
}

int file_println   (FILE *self, const char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	result = file_vprint(self, format, args);
	result += file_print(self, "\n");
	va_end(args);
	return result;
}


void file_close(FILE *self) {
	if (fclose(self) != 0)
		throw(new(IOError(), strerror(errno)));
}

int file_vprint(FILE *self, const char *format, va_list args) {
	int result = 0;
	int i = 0;

	// TODO: allow to j lustify (e.g. print("%*O", 15, list(1,2,3,4))

	while ( format[i] ) {
		if (format[i] == '%') {
			if (format[i+1] == 'O') {
				int j = parse_curly_braces(&format[i+2]);
				if (j > 0) {
					char *subformat = string_slice(format, i+3, i+j+2);
					result += put(va_arg(args, void *), self, subformat);
					delete(subformat);
					i += 2+j+1;
				}
				else {
					result += put(va_arg(args, void *), self, NULL);
					i += 2;
				}
			}
			else {
				int j = parse_format(&format[i+1]);
				char *subformat = string_slice(format, i, i+j+2);
				result += vfprintf(self, subformat, args);

				// skip the necessary number of argument
				int a, n = parse_printf_format(subformat, 0, NULL);
				for (a = 0; a < n; ++a)
					(void)va_arg(args, void *);

				delete(subformat);
				i += j+2;
			}
		}
		else {
			putc(format[i], self);
			result += 1;
			i += 1;
		}
	}
	return result;
}

