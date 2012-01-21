#include <sool/string.h>

struct _string_t {
	const class_t *class;
	char *value;
	int length;
};

static void string_ctor(void *_self, va_list *args) {
	string_t *self = _self;
	self->length = va_arg(*args, int);
	self->value  = strndup(va_arg(*args, char *), self->length);
}

static void string_dtor(void *_self) {
	string_t *self = _self;
	free(self->value);
}

static int string_put(void *_self, FILE *stream, const char *format) {
	(void)format;
	string_t *self = _self;
	return fprintf(stream, "\"%s\"", self->value);
}

static int string_equal(void *_self, void *_other) {
	string_t *self = _self, *other = _other;
	return strcmp(self->value, other->value);
}

const class_t *String() {
	static const class_t *result = NULL;
	return result ? result : (result = class(sizeof(string_t), __FUNCTION__, string_ctor, string_dtor, string_put, string_equal));
}

string_t *string_clone(string_t *self) {
	return new(String(), self->length, self->value);
}

// TODO add string_slice

// TODO add string_join

// TODO add string_join_list

//

int string_length(string_t *self) {
	return self->length;
}

char *string_cstr(string_t *self) {
	return self->value;
}

list_t *string_split(string_t *self, const char *delimiter) {
	list_t *result = list();
	char *current = string_cstr(self), *next;
	int delimiter_length = strlen(delimiter);

	while ( (next = strstr(current, delimiter)) != NULL ) {
		list_append(result, new(String(), next-current, current));
		current = next + delimiter_length;
	}
	list_append(result, string(current));

	return result;
}

int string_find(string_t *self, string_t *substr) {
	char *tmp = strstr(self->value, substr->value);
	return (tmp == NULL ? -1 : tmp - self->value);
}

static int string_index(string_t *self, int i) {
	return i < 0 ? self->length + i : i;
}

char string_get(string_t *self, int i) {
	return self->value[string_index(self, i)];
}

char string_set(string_t *self, int i, char c) {
	return (self->value[string_index(self, i)] = c);
}
