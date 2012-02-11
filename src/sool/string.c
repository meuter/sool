#include <sool/string.h>
#include <sool/exception.h>
#include <sool/mem.h>

/*****************************************************************************/

static const char *string(const char *self) {
	if (self == NULL)
		throw(new(NullPointerError(), ""));
	return self;
}

static int string_index(const char *self, int i) {
	int n = string_length(self);
	if ( i > n || i < -n )
		throw(new(IndexError(), ""));
	return i < 0 ? n + i : i;
}

static char *xstrndup(const char *s, int n) {
	char *result = xmalloc(n);
	return strncpy(result, s, n);
}

/*****************************************************************************/

char * string_clone(const char *self) {
	return xstrndup(self, string_length(self));
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
		list_append(result, xstrndup(self, i));
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
	char *result = xmalloc(to - from);
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
//TODO char   *string_center      (const char *self, int width, char filler);
//TODO char   *string_lower       (const char *self);
//TODO char   *string_upper       (const char *self);
//TODO char   *string_title       (const char *self);
//TODO bool_t  string_starts_with (const char *self, const char *substr);
//TODO bool_t  string_ends_with   (const char *self, const char *substr);



