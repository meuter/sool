#include <sool/dict.h>
#include <sool/list.h>
#include <sool/mem.h>
#include <sool/io.h>

#include "object_def.h"

typedef struct {
	void *key;
	void *value;
} dict_entry_t ;

struct _dict_t {
	EXTENDS(object_t);
	list_t **buckets;
	int n_buckets;
	list_t *entries;
	hash_function_t hash;
};

static const int DEFAULT_N_BUCKETS = 256;

static void *dict_ctor(void *_self, va_list *args) {
	dict_t *self = super_ctor(Dictionary(), _self, args);
	int n, i;

	self->hash = va_arg(*args, hash_function_t);
	self->buckets = mem_alloc(DEFAULT_N_BUCKETS * sizeof(list_t *));
	self->n_buckets = DEFAULT_N_BUCKETS;
	self->entries = list();

	for (i = 0; i < self->n_buckets; ++i)
		self->buckets[i] = NULL;

	n = va_arg(*args, int);

	for (i = 0; i < n; ++i) {
		void *key   = va_arg(*args, void *);
		void *value = va_arg(*args, void *);
		dict_put(self, key, value);
	}

	return self;
}

static void *dict_dtor(void *_self) {
	dict_t *self = super_dtor(Dictionary(), _self);
	int i;
	for (i = 0; i < DEFAULT_N_BUCKETS; ++i) {
		if (self->buckets[i]) {
			iterator_t *j;
			forall(j, self->buckets[i])
				delete(get(j));
			delete(self->buckets[i]);
		}
	}
	return self;
}


static int dict_put_(void *_self, FILE *stream, const char *format) {
	dict_t *self = super_dtor(Dictionary(), _self);
	iterator_t *i;
	dict_entry_t *entry;
	int result = 0;

	if (format == NULL) format = "%p: %p";

	result += file_print(stream, "{");
	if (!list_is_empty(self->entries)) {
		for (i = begin(self->entries); i != previous(end(self->entries)); i = next(i)) {
			entry = get(i);
			result += file_print(stream, format, entry->key, entry->value);
			result += file_print(stream, ", ");
		}
		entry = get(rbegin(self->entries));
		result += file_print(stream, format, entry->key, entry->value);
	}
	result += file_print(stream, "}");
	return result;
}

class_t *Dictionary() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(Class(), "Dictionary", Object(), sizeof(dict_t),
					ctor, dict_ctor,
					dtor, dict_dtor,
					put,  dict_put_,
					NULL
		);
	}
	return result;
}


void *dict_put(dict_t *self, void *key, void *value) {
	self = cast(Dictionary(), self);
	int h = self->hash(key) % self->n_buckets;
	if (self->buckets[h] == NULL)
		self->buckets[h] = list();

	iterator_t *i;
	forall(i,self->buckets[h]) {
		dict_entry_t *e = get(i);
		if (equal(e->key, key)) {
			e->value = value;
			return value;
		}
	}

	dict_entry_t *dict_entry = mem_alloc(sizeof(dict_entry_t));
	dict_entry->key = key;
	dict_entry->value = value;

	list_append(self->buckets[h], dict_entry);
	list_append(self->entries, dict_entry);
	return value;
}
