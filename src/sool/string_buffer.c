#include <sool/string_buffer.h>
#include <sool/exception.h>
#include <sool/mem.h>

#include "object_def.h"

const int STRING_BUFFER_DEFAULT_SIZE = 256;

/*****************************************************************************/

struct _string_buffer {
	EXTENDS(object_t);
	char *buffer;
	size_t size, length;
};

static void *string_buffer_ctor(void *_self, va_list *args) {
	string_buffer_t *self = super_ctor(StringBuffer(), _self, args);
	self->size      = STRING_BUFFER_DEFAULT_SIZE;
	self->buffer    = mem_alloc(self->size);
	self->buffer[0] = 0;
	self->length    = 0;
	return self;
}

static void *string_buffer_dtor(void *_self) {
	string_buffer_t *self = super_dtor(StringBuffer(), _self);
	mem_free(self->buffer);
	return self;
}

class_t *StringBuffer() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), __FUNCTION__, Object(), sizeof(string_buffer_t),
			ctor, string_buffer_ctor,
			dtor, string_buffer_dtor,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

size_t string_buffer_length(string_buffer_t *self) {
	self = cast(StringBuffer(), self);
	return self->length;
}

size_t string_buffer_size(string_buffer_t *self) {
	self = cast(StringBuffer(), self);
	return self->size;
}

int string_buffer_append(string_buffer_t *self, const char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	result = string_buffer_vappend(self, format, args);
	va_end(args);
	return result;
}

int string_buffer_vappend(string_buffer_t *self, const char *format, va_list args) {
	int result;
	va_list copy;

	self = cast(StringBuffer(), self);
	va_copy(copy, args);

	if ( (result = vsnprintf(NULL, 0, format, copy)) < 0 )
		throw(new(MemoryError(),""));

	size_t needed = self->length + result + 1;
	if ( self->size < needed ) {
		while ( self->size < needed )
			self->size = (self->size + 1) * 2;
		self->buffer = mem_realloc(self->buffer, self->size);
	}

	size_t remaining = self->size - self->length;
	if ( vsnprintf(self->buffer + self->length, remaining, format, args) != result )
		throw(new(MemoryError(),""));

	self->length += result;

	return result;
}

void string_buffer_detach(string_buffer_t *self) {
	self = cast(StringBuffer(), self);
	self->size   = 0;
	self->buffer = NULL;
	self->length = 0;
}

char *string_buffer_value(string_buffer_t *self) {
	self = cast(StringBuffer(), self);
	return self->buffer;
}

void string_buffer_reset(string_buffer_t *self) {
	self = cast(StringBuffer(), self);
	self->buffer[0] = 0;
	self->length    = 0;
}

