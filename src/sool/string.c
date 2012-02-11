#include <sool/string.h>
#include <sool/exception.h>
#include <sool/args.h>
#include <sool/mem.h>

#include <stdio.h>

/*****************************************************************************/

static const char *string(const char *self) {
	if (self == NULL)
		throw(new(NullPointerError()));
	return self;
}

static int string_index(const char *self, int i) {
	int n = string_length(self);
	if ( i > n || i < -n )
		throw(new(IndexError()));
	return i < 0 ? n + i : i;
}


static char *string_paste(char *s, const char *t) {
	t = string(t);
	while ( (*s++ = *t++) )
		;
	*--s = 0;
	return s;
}

static char *string_dup(const char *s, int n) {
	char *result = mem_alloc(n+1);
	strncpy(result, s, n);
	result[n] = 0;
	return result;
}
/*****************************************************************************/

char * string_clone(const char *self) {
	return string_dup(self, string_length(self));
}

size_t string_length(const char *self) {
	return strlen(string(self));
}

char string_get(const char *self, int i) {
	return self[string_index(self, i)];
}

int string_find(const char *self, const char *substr) {
	const char *tmp = strstr(string(self), string(substr));
	return (tmp == NULL ? -1 : tmp - self);
}

// TODO rfind

bool_t string_contains(const char *self, const char *substr) {
	return (string_find(self, substr) != -1);
}


char *string_strip(const char *self) {
#define WS(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	int from = 0, to = string_length(self);
	while (WS(self[from])) from++;
	while (WS(self[to-1])) to--;
#undef WS
	return string_slice(self, from, to);
}


list_t *string_split(const char *self, const char *delimiter) {
	list_t *result = list();
	int i, n = string_length(delimiter);

	while ( (i = string_find(self, delimiter)) != -1 ) {
		list_append(result, string_dup(self, i));
		self = self + i + n;
	}
	list_append(result, string_clone(self));

	return result;
}

char *_string_join(const char *self, int n, ...) {
	int length, i;
	char *result, *ptr;
	va_list args;

	self = string(self);

	if (n == 0)	return string_clone("");

	// compute the length of the resulting string
	length = string_length(self) * (n - 1);
	va_start(args, n);
	for (i = 0; i < n; ++i)
		length += string_length(va_arg(args, const char *));
	va_end(args);

	// build the concatenated string
	ptr = result = mem_alloc(length + 1);

	// this does not throw any exception, since the list of args
	// is checked by the first loop
	va_start(args, n);
	for (i = 0; i < n-1; ++i) {
		ptr = string_paste(ptr, va_arg(args, const char *));
		ptr = string_paste(ptr, self);
	}
	ptr = string_paste(ptr, va_arg(args, const char *));
	va_end(args);

	return result;
}

char *string_slice(const char *self, int from, int to) {
	from = string_index(self, from);
	to   = string_index(self, to);
	if (from >= to) return string_clone("");
	return string_dup(self + from, to - from);
}

// TODO replace

int string_cmp(const char *self, const char *other) {
	return strcmp(string(self), string(other));
}

bool_t string_equal(const char *self, const char *other) {
	return string_cmp(self, other) == 0;
}

static char *string_typeset(const char *self, int length, int left, int right, char filler) {
	int i, n = length+left+right;
	if (n < length) return string_clone(self);
	char *result = mem_alloc(n+1);
	for (i = 0; i < left; ++i) result[i] = filler;
	memcpy(result+left, self, length);
	for (i += length; i < n; ++i) result[i] = filler;
	result[n] = '\0';
	return result;
}

char *string_ljust(const char *self, int width, char filler) {
	int length = string_length(self);
	return string_typeset(self, length, 0, width - length, filler);
}

char *string_rjust(const char *self, int width, char filler) {
	int length = string_length(self);
	return string_typeset(self, length, width - length, 0, filler);
}

char *string_center(const char *self, int width, char filler) {
	int length = string_length(self);
	return string_typeset(self, length, (width - length)/2, width - length - (width - length)/2, filler);
}

char *string_lower(const char *self) {
	int length = string_length(self), i;
	char *result = string_dup(self, length);

	for (i = 0; i < length; ++i) {
		if (result[i] >= 'A' && result[i] <= 'Z')
			result[i] += 'a'-'A';
	}
	return result;
}


char *string_upper(const char *self) {
	int length = string_length(self), i;
	char *result = string_dup(self, length);

	for (i = 0; i < length; ++i) {
		if (result[i] >= 'a' && result[i] <= 'z')
			result[i] += 'A'-'a';
	}
	return result;
}


//TODO char   *string_title       (const char *self);

bool_t  string_starts_with (const char *self, const char *substr) {
	int n = string_length(self), m = string_length(substr);
	if (n < m) return FALSE;
	return (memcmp(self, substr, m) == 0);
}

bool_t  string_ends_with   (const char *self, const char *substr) {
	int n = string_length(self), m = string_length(substr);
	if (n < m) return FALSE;
	return (memcmp(self + n - m, substr, m) == 0);
}

char *string_format(const char *self, ...) {
	int n;
	va_list args, copy;
	char *result;

	self = string(self);
	va_start(args, self);
	va_copy(copy, args);

	if ( (n = vsnprintf(NULL, 0, self, copy)) < 0 )
		throw(new(MemoryError()));

	result = mem_alloc(n+1);
	if ( vsnprintf(result, n+1, self, args) != n ) {
		mem_free(result);
		throw(new(MemoryError()));
	}

	va_end(args);

	return result;
}




