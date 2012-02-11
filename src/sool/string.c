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

static char *string_dup(const char *s, int n) {
	char *result = mem_alloc(n+1);
	return strncpy(result, s, n);
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

// TODO join

char *string_slice(const char *self, int from, int to) {
	from = string_index(self, from);
	to   = string_index(self, to);
	if (from >= to) return string_clone("");
	char *result = mem_alloc(to - from);
	return strncpy(result, self + from, to - from);
}

// TODO replace

int string_cmp(const char *self, const char *other) {
	return strcmp(string(self), string(other));
}

bool_t string_equal(const char *self, const char *other) {
	return string_cmp(self, other) == 0;
}

//TODO char   *string_ljust       (const char *self, int width, char filler);
//TODO char   *string_rjust       (const char *self, int width, char filler);

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




//TODO char   *string_lower       (const char *self);
//TODO char   *string_upper       (const char *self);
//TODO char   *string_title       (const char *self);
//TODO bool_t  string_starts_with (const char *self, const char *substr);
//TODO bool_t  string_ends_with   (const char *self, const char *substr);

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




