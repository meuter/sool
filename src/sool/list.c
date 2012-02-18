#include <sool/list.h>
#include <sool/mem.h>
#include <sool/io.h>
#include <sool/sequence.h>

#include "sequence_def.h"

/*****************************************************************************/

class_t *ListIterator();

struct _list_iterator_t {
	EXTENDS(object_t);
	void *value;
	list_iterator_t *next, *previous;
};

static void *list_iterator_next(void *_self) {
	list_iterator_t *self = cast(ListIterator(), _self);
	return self->next;
}

static void *list_iterator_previous(void *_self) {
	list_iterator_t *self = cast(ListIterator(), _self);
	return self->previous;
}

static void *list_iterator_get(void *_self) {
	list_iterator_t *self = cast(ListIterator(), _self);
	return self->value;
}

static void *list_iterator_ctor(void *_self, va_list *args) {
	list_iterator_t *self = super_ctor(ListIterator(), _self, args);

	void *previous = va_arg(*args, void *);
	void *value    = va_arg(*args, void *);
	void *next     = va_arg(*args, void *);

	self->previous = previous ? cast(ListIterator(), previous) : previous;
	self->value    = value;
	self->next     = next ? cast(ListIterator(), next) : next;
	return self;
}

class_t *ListIterator() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(IteratorClass(), __FUNCTION__, Object(), sizeof(list_iterator_t),
			ctor,     list_iterator_ctor,
			get,      list_iterator_get,
			next,     list_iterator_next,
			previous, list_iterator_previous,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/


struct _list_t {
	EXTENDS(object_t);
	list_iterator_t *dummy;
	int length;
};

static void *list_ctor(void *_self, va_list *args) {
    list_t *self = super_ctor(List(), _self, args);
    int i, n;

    self->dummy = new(ListIterator(), NULL, 0, NULL);
    self->dummy->next = self->dummy->previous = self->dummy;
    self->length = 0;

    n = va_arg(*args, int);
    for (i = 0; i < n; ++i)
    	list_append(self, va_arg(*args, void *));

    return self;
}

static void *list_dtor(void *_self) {
	list_t *self = super_dtor(List(), _self);
	while (!list_is_empty(self))
		list_remove_first(self);
	mem_free(self->dummy);
	return self;
}

static int list_put(void *_self, FILE *stream, const char *format) {
	int result = 0;
	list_t *self = cast(List(), _self);
	iterator_t *i;

	if (format == NULL)
		format = "%p";

	result += fprintf(stream, "[");
	forall(i, self) {
		result += file_print(stream, format, get(i));
		result += fprintf(stream, next(i) != end(self) ? ", " : "");
	}
	result += fprintf(stream, "]");
	return result;
}


// TODO need to find a way to compare elements other than identity
// TODO extend the cmp function to deal with the case < and >
static int list_cmp(void *_self, void *_other) {
	list_t *self = cast(List(),_self), *other = cast(List(), _other);
	iterator_t *i, *j;

	if (self == other)
		return 0;
	if (list_length(self) != list_length(other))
		return -1;

	for (i = begin(self), j = begin(other); i != end(self); i = next(i), j = next(j)) {
		if (get(i) != get(j))
			return -1;
	}

	return 0;
}

static iterator_t *list_begin(void *_self) {
	list_t *self = cast(List(), _self);
	return (iterator_t *)self->dummy->next;
}

static iterator_t *list_end(void *_self) {
	list_t *self = cast(List(), _self);
	return (iterator_t *)self->dummy;
}

static iterator_t *list_rbegin(void *_self) {
	list_t *self = cast(List(), _self);
	return (iterator_t *)self->dummy->previous;
}

static iterator_t *list_rend(void *_self) {
	list_t *self = cast(List(), _self);
	return (iterator_t *)self->dummy;
}

class_t *List() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(SequenceClass(), __FUNCTION__, Object(), sizeof(list_t),
			ctor,  list_ctor,
			dtor,  list_dtor,
			put,   list_put,
			cmp,   list_cmp,
			begin, list_begin,
			end,   list_end,
			rbegin, list_rbegin,
			rend,   list_rend,
			NULL
		);
	}
	return result;
}

/*****************************************************************************/

list_t *list_clone(list_t *self) {
	return list_copy(self, begin(self), end(self));
}

list_t  *list_copy(list_t *self, iterator_t *from, iterator_t *to) {
	(void)self;
	list_t *result = list();
	iterator_t *i;

	for (i = from; i != to; i = next(i))
		list_append(result, get(i));

	return result;
}

list_t *list_slice(list_t *self, int from, int to) {
	return list_copy(self, list_get(self, from), list_get(self, to));
}

list_t *_list_join(int n, ...) {
	list_t *result = list(), *tmp;
	iterator_t *i;
	va_list args;

	va_start(args, n);
	while (n--) {
		tmp = cast(List(), va_arg(args, list_t *));
		forall(i, tmp)
			list_append(result, get(i));
	}
	va_end(args);

	return result;
}

// FIXME list_sort does not work!
list_t *list_sort(list_t *self) {
	list_t *result = list();
	iterator_t *i, *j;

	forall(i, self) {
		void *x = get(i);

		j = begin(result);
		while (j != end(result) && x >= get(j))
			j = next(j);
		list_insert_after(result, j, x);
	}

	return result;
}

list_t *list_reverse(list_t *self) {
	list_t *result = list();
	iterator_t *i;

	forall(i, self)
		list_prepend(result, get(i));

	return result;
}

bool_t list_is_empty(list_t *self) {
	return (self->length == 0);
}


int list_count(list_t *self, void *value) {
	iterator_t *i;
	int result = 0;
	forall(i, self)
		result += (get(i) == value) ? 1 : 0;
	return result;
}

int list_length(list_t *self) {
	self = cast(List(), self);
	return self->length;
}

void list_clear(list_t *self) {
	while (!list_is_empty(self))
		list_remove_first(self);
}


iterator_t *list_get(list_t *self, int i) {
	iterator_t *j;

	if ( i >= 0 ) {
		j = begin(self);
		while (i--) {
			if ( j == end(self) ) break;
			j = next(j);
		}
	}
	else {
		j = rbegin(self);
		while (i++ < -1) {
			if ( j == rend(self) ) break;
			j = previous(j);
		}
	}
	return j;
}

iterator_t *list_find(list_t *self, void *value) {
	return find(self, value);
}

iterator_t *list_rfind(list_t *self, void *value) {
	return rfind(self, value);
}


iterator_t *list_insert_before(list_t *self, iterator_t *_i, void *value) {
	list_iterator_t *i = cast(ListIterator(), _i);
	list_iterator_t *new_item = new(ListIterator(), i, value, i->next);

	new_item->next->previous = new_item;
	new_item->previous->next = new_item;

	self->length++;
	return new_item;
}


iterator_t *list_insert_after(list_t *self, iterator_t *_i, void *value) {
	list_iterator_t *i = cast(ListIterator(), _i);
	list_iterator_t *new_item = new(ListIterator(), i->previous, value, i);

	new_item->previous->next = new_item;
	new_item->next->previous = new_item;

	self->length++;
	return new_item;
}

iterator_t *list_prepend(list_t *self, void *value) {
	return list_insert_before(self, list_rend(self), value);
}

iterator_t *list_append(list_t *self, void *value) {
	return list_insert_after(self, end(self), value);
}

void *list_remove(list_t *self, iterator_t *_to_remove) {
	list_iterator_t *to_remove = cast(ListIterator(), _to_remove);
	to_remove->previous->next = to_remove->next;
	to_remove->next->previous = to_remove->previous;
	void *result = to_remove->value;
	mem_free(to_remove);
	self->length--;
	return result;
}

void *list_remove_first(list_t *self) {
	return list_remove(self, begin(self));
}

void *list_remove_last(list_t *self) {
	return list_remove(self, end(self));
}






