#include <sool/object.h>
#include <sool/io.h>
#include <sool/bool.h>
#include <string.h>
#include <assert.h>

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

static int parse_curly_braces(const char *string) {
	int i = 0;
	int n_open = 0;

	if (string[0] == '}') return -1;
	if (string[0] != '{') return 0;

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


int ovfprintf(FILE *stream, const char *format, va_list args) {
	int result = 0;
	int i = 0;
	char subformat[16];

	while ( format[i] ) {
		if (format[i] == '%') {
			if (format[i+1] == 'O') {
				int j = parse_curly_braces(&format[i+2]);
				if (j > 0) {
					memcpy(subformat, &format[i+3], j-1);
					subformat[j-1] = '\0';
					result += put(va_arg(args, void *), stream, subformat);
					i += 2+j+1;
				}
				else {
					result += put(va_arg(args, void *), stream, "");
					i += 2;
				}

			}
			else {
				int j = i+1;

				while (!is_format_character(format[j]))
					j++;
				j++;

				assert(j-i < 16);
				memcpy(subformat, &format[i], j-i);
				subformat[j-i] = '\0';
				result += vfprintf(stream, subformat, args);
				i = j;
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
