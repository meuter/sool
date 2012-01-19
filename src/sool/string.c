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

static bool_t string_equal(void *_self, void *_other) {
	string_t *self = _self, *other = _other;
	return (strcmp(self->value, other->value) == 0);
}

const class_t *String() {
	static const class_t *result = NULL;
	return result ? result : (result = class(sizeof(string_t), __FUNCTION__, string_ctor, string_dtor, string_put, string_equal));
}

char *string_cstr(string_t *self) {
	return self->value;
}

int string_length(string_t *self) {
	return self->length;
}

string_t *string_clone(string_t *self) {
	return new(String(), self->length, self->value);
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
