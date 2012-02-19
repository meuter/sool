#include <sool/string.h>
#include <sool/exception.h>
#include <sool/args.h>
#include <sool/mem.h>
#include <sool/string_buffer.h>

#include <stdio.h>

#include "exception_def.h"

DEFINE_EXCEPTION(IndexError);

/*****************************************************************************/

static bool is_space(const char c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static bool is_lower(const char c) {
	return (c >= 'a' && c <= 'z');
}

static bool is_upper(const char c) {
	return (c >= 'A' && c <= 'Z');
}

static bool is_alpha(const char c) {
	return (is_lower(c) || is_upper(c));
}

static bool is_num(const char c) {
	return (c >= '0' && c <= '9');
}

static bool is_alphanum(const char c) {
	return (is_alpha(c) || is_num(c));
}

static bool string_forall(const char *self, bool (*check)(const char)) {
	int length = string_length(self), i;
	for (i = 0; i < length; ++i)
		if (!check(self[i]))
			return false;
	return true;
}

static bool string_exist(const char *self, bool (*check)(const char)) {
	int length = string_length(self), i;
	for (i = 0; i < length; ++i)
		if (check(self[i]))
			return true;
	return false;
}

/*****************************************************************************/

static const char *string(const char *self) {
	if (self == NULL) throw(new(NullPointerError(),""));
	return self;
}

static int string_index(const char *self, int i) {
	int n = string_length(self);
	if ( i > n || i < -n )
		throw(new(IndexError(),""));
	return i < 0 ? n + i : i;
}

static char *string_dup(const char *s, int n) {
	char *result = mem_alloc(n+1);
	strncpy(result, s, n);
	result[n] = 0;
	return result;
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

int string_rfind(const char* self, const char* substr) {
	int substr_length = string_length(substr);
	const char *end = self + string_length(self) - substr_length;
	const char *tmp;

	for(tmp = end; tmp >= self; --tmp) {
		if (memcmp(tmp, substr, substr_length) == 0)
			return tmp - self;
	}
	return -1;
}

bool string_contains(const char *self, const char *substr) {
	return (string_find(self, substr) != -1);
}

char *string_strip(const char *self) {
	int from = 0, to = string_length(self);
	while (is_space(self[from])) from++;
	while (is_space(self[to-1])) to--;
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

char *string_join(const char *self, list_t *words, const char *format) {
	self = string(self);

	// if list is empty
	if (list_length(words) == 0) return string_clone("");

	// create new buffer
	string_buffer_t *buffer = new(StringBuffer());
	iterator_t *i;

	// check for non-NULL string
	forall(i,words)	string(get(i));

	if (format == NULL) format = "%p";

	// build the string in the buffer
	for (i = begin(words); i != previous(end(words)); i = next(i)) {
		string_buffer_append(buffer, format, get(i));
		string_buffer_append(buffer, "%s", self);
	}
	string_buffer_append(buffer, format, get(rbegin(words)));

	// detach buffer and delete buffer
	char *result = string_buffer_value(buffer);
	string_buffer_detach(buffer);
	delete(buffer);

	// return result
	return result;
}

char *string_slice(const char *self, int from, int to) {
	from = string_index(self, from);
	to   = string_index(self, to);
	if (from >= to) return string_clone("");
	return string_dup(self + from, to - from);
}

char *string_replace(const char *self, const char *substr, const char *other) {
	other = string(other);
	list_t *splitted = string_split(self, substr);
	iterator_t *i;
	char *result = string_join(other, splitted, "%s");
	forall(i, splitted) delete(get(i));
	return result;
}

int string_cmp(const char *self, const char *other) {
	return strcmp(string(self), string(other));
}

bool string_equal(const char *self, const char *other) {
	return string_cmp(self, other) == 0;
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
		if (is_upper(result[i]))
			result[i] += 'a'-'A';
	}
	return result;
}

char *string_upper(const char *self) {
	int length = string_length(self), i;
	char *result = string_dup(self, length);

	for (i = 0; i < length; ++i) {
		if (is_lower(result[i]))
			result[i] += 'A'-'a';
	}
	return result;
}

bool string_starts_with(const char *self, const char *substr) {
	int n = string_length(self), m = string_length(substr);
	if (n < m) return false;
	return (memcmp(self, substr, m) == 0);
}

bool string_ends_with(const char *self, const char *substr) {
	int n = string_length(self), m = string_length(substr);
	if (n < m) return false;
	return (memcmp(self + n - m, substr, m) == 0);
}

char *string_format(const char *self, ...) {
	self = string(self);
	string_buffer_t *buffer = new(StringBuffer());
	va_list args;
	va_start(args, self);
	string_buffer_vappend(buffer, self, args);
	va_end(args);
	char *result = string_buffer_value(buffer);
	string_buffer_detach(buffer);
	delete(buffer);
	return result;
}

bool string_is_space(const char *self) {
	return string_forall(self, is_space) && (*self != 0);
}

bool string_is_lower(const char *self) {
	return !string_exist(self, is_upper) && string_exist(self, is_lower);
}

bool string_is_upper(const char *self) {
	return !string_exist(self, is_lower) && string_exist(self, is_upper);
}

bool string_is_alpha(const char *self) {
	return string_forall(self, is_alpha) && (*self != 0);
}

bool string_is_num(const char *self) {
	return string_forall(self, is_num) && (*self != 0);
}

bool string_is_alphanum(const char *self) {
	return string_forall(self, is_alphanum) && (*self != 0);
}
