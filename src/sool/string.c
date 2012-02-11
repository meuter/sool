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
	if ( i >= n || i < -n )
		throw(new(IndexError(), ""));
	return i < 0 ? n + i : i;
}

/*****************************************************************************/

size_t string_length(const char *self) {
	return strlen(string(self));
}

char * string_clone(const char *self) {
	return xstrdup(string(self));
}

char string_get(const char *self, int i) {
	return self[string_index(self, i)];
}

char string_set(char *self, int i, char c) {
	return (self[string_index(self, i)] = c);
}

int string_cmp(const char *self, const char *other) {
	return strcmp(string(self), string(other));
}

bool_t string_equal(const char *self, const char *other) {
	return string_cmp(self, other) == 0;
}

//
//class_t *String() {
//	static class_t *result = NULL;
//	if (result == NULL) {
//		result = new(Class(), "String", Class(), sizeof(string_t),
//			ctor, string_ctor,
//			dtor, string_dtor,
//			put, string_put,
//			cmp, string_cmp,
//			NULL
//		);
//	}
//	return result;
//}
//
//string_t *string_clone(string_t *self) {
//	return new(String(), self->length, self->value);
//}
//
//// TODO add string_slice
//
//// TODO add string_join
//
//// TODO add string_join_list
//
//char *string_cstr(string_t *self) {
//	return self->value;
//}
//
//list_t *string_split(string_t *self, const char *delimiter) {
//	list_t *result = list();
//	char *current = string_cstr(self), *next;
//	int delimiter_length = strlen(delimiter);
//
//	while ( (next = strstr(current, delimiter)) != NULL ) {
//		list_append(result, new(String(), next-current, current));
//		current = next + delimiter_length;
//	}
//	list_append(result, string(current));
//
//	return result;
//}
//
//int string_find(string_t *self, string_t *substr) {
//	char *tmp = strstr(self->value, substr->value);
//	return (tmp == NULL ? -1 : tmp - self->value);
//}
//
//
//
//
//bool_t string_contains (string_t *self, const char *substr) {
//	return (strstr(self->value, substr) != NULL);
//}
