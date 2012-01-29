#include <sool/list.h>
#include <sool/mem.h>
#include <sool/io.h>
#include <sool/sequence.h>

#include "object_def.h"

struct _list_iterator_t {
	EXTENDS(object_t);
	void *value;
	list_iterator_t *next, *previous;
};

struct _list_t {
	EXTENDS(object_t);
	list_iterator_t *dummy;
	int length;
};

class_t *ListIterator();


static void *list_ctor(void *_self, va_list *args) {
    list_t *self = _self;
    int i, n;

    self->dummy = new(ListIterator());
    self->dummy->next = self->dummy->previous = self->dummy;
    self->length = 0;

    n = va_arg(*args, int);
    for (i = 0; i < n; ++i)
    	list_append(self, va_arg(*args, void *));

    return self;
}

static void *list_dtor(void *_self) {
	list_t *self = _self;
	while (!list_is_empty(self))
		list_remove_first(self);
	xfree(self->dummy);
	return self;
}

static int list_put(void *_self, FILE *stream, const char *format) {
	int result = 0;
	list_t *self = _self;
	list_iterator_t *i;

	if (format == NULL)
		format = "%p";

	result += fprintf(stream, "[");
	forall(i, self) {
		result += ofprintf(stream, format, get(i));
		result += fprintf(stream, next(i) != end(self) ? ", " : "");
	}
	result += fprintf(stream, "]");
	return result;
}


// TODO need to find a way to compare elements other than identity
// TODO extend the cmp function to deal with the case < and >
static int list_cmp(void *_self, void *_other) {
	list_t *self = _self, *other = _other;
	list_iterator_t *i, *j;

	if (self == other)
		return 0;
	if (list_length(self) != list_length(other))
		return -1;

	for (i = list_begin(self), j = list_begin(other); i != list_end(self); i = item_next(i), j = item_next(j)) {
		if (item_get(i) != item_get(j))
			return -1;
	}

	return 0;
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
			NULL
		);
	}
	return result;
}

static void *list_iterator_next(void *_self) {
	list_iterator_t *self = cast(ListIterator(), _self);
	return self->next;
}

static void *list_iterator_get(void *_self) {
	list_iterator_t *self = cast(ListIterator(), _self);
	return self->value;

}


class_t *ListIterator() {
	static class_t *result = NULL;
	if (result == NULL) {
		result = new(IteratorClass(), __FUNCTION__, Object(), sizeof(list_iterator_t),
			get,  list_iterator_get,
			next, list_iterator_next,
			NULL
		);
	}
	return result;
}


list_t *list_clone(list_t *self) {
	return list_copy(self, list_begin(self), list_end(self));
}

list_t *list_copy(list_t *self, list_iterator_t *from, list_iterator_t *to) {
	(void)self;
	list_t *result = list();
	list_iterator_t *i = from;
	while (i != to) {
		list_append(result, item_get(i));
		i = item_next(i);
	}

	return result;
}

list_t *list_slice(list_t *self, int from, int to) {
	return list_copy(self, list_get(self, from), list_get(self, to));
}

list_t *_list_join(int n, ...) {
	list_t *result = list(), *tmp;
	list_iterator_t *i;
	va_list args;

	va_start(args, n);
	while (n--) {
		tmp = va_arg(args, list_t *);
		forall(i, tmp)
			list_append(result, item_get(i));
	}
	va_end(args);

	return result;
}



list_t *list_sort(list_t *self) {
	list_t *result = list();
	list_iterator_t *i, *j;

	forall(i, self) {
		void *x = item_get(i);

		j = list_begin(result);
		while (j != list_end(result) && x >= item_get(j))
			j = item_next(j);
		list_insert_after(result, j, x);
	}

	return result;
}


list_t *list_reverse(list_t *self) {
	list_t *result = list();
	list_iterator_t *i;
	forall(i, self)
		list_prepend(result, item_get(i));
	return result;
}

bool_t list_is_empty(list_t *self) {
	return (self->length == 0);
}

int list_count(list_t *self, void *value) {
	list_iterator_t *i;
	int result = 0;
	forall(i, self)
		result += (item_get(i) == value) ? 1 : 0;
	return result;
}

int list_length(list_t *self) {
	return self->length;
}

void list_clear(list_t *self) {
	while (!list_is_empty(self))
		list_remove_first(self);
}

list_iterator_t *list_get(list_t *self, int i) {
	list_iterator_t *j;

	if ( i >= 0 ) {
		j = list_begin(self);
		while (i--) {
			if ( j == list_end(self) ) break;
			j = item_next(j);
		}
	}
	else {
		j = list_rbegin(self);
		while (i++ < -1) {
			if ( j == list_rend(self) ) break;
			j = item_previous(j);
		}
	}
	return j;
}

list_iterator_t *list_set(list_t *self, int i, void *value) {
	list_iterator_t *result = list_get(self, i);
	item_set(result, value);
	return result;
}

list_iterator_t *list_find(list_t *self, void *value) {
	list_iterator_t *i;
	forall(i, self) {
		if (item_get(i) == value)
			break;
	}
	return i;
}

list_iterator_t *list_rfind(list_t *self, void *value) {
	list_iterator_t *i;
	for (i = list_rbegin(self); i != list_rend(self); i = item_previous(i)) {
		if (item_get(i) == value)
			break;
	}
	return i;
}

list_iterator_t *list_insert_before(list_t *self, list_iterator_t *item, void *value) {
	list_iterator_t *new_item = new(ListIterator());

	new_item->next = item->next;
	new_item->next->previous = new_item;
	new_item->value = value;
	new_item->previous = item;
	new_item->previous->next = new_item;

	self->length++;
	return new_item;
}


list_iterator_t *list_insert_after(list_t *self, list_iterator_t *item, void *value) {
	list_iterator_t *new_item = new(ListIterator());

	new_item->previous = item->previous;
	new_item->previous->next = new_item;
	new_item->value = value;
	new_item->next = item;
	new_item->next->previous = new_item;

	self->length++;
	return new_item;
}

list_iterator_t *list_prepend(list_t *self, void *value) {
	return list_insert_before(self, list_rend(self), value);
}

list_iterator_t *list_append(list_t *self, void *value) {
	return list_insert_after(self, list_end(self), value);
}

list_iterator_t *list_begin(list_t *self) {
	return self->dummy->next;
}

list_iterator_t *list_end(list_t *self) {
	return self->dummy;
}

list_iterator_t *list_rbegin(list_t *self) {
	return self->dummy->previous;
}

list_iterator_t *list_rend(list_t *self) {
	return self->dummy;
}


void *list_remove(list_t *self, list_iterator_t *to_remove) {
	to_remove->previous->next = to_remove->next;
	to_remove->next->previous = to_remove->previous;
	void *result = to_remove->value;
	xfree(to_remove);
	self->length--;
	return result;
}

void *list_remove_first(list_t *self) {
	return list_remove(self, list_begin(self));
}

void *list_remove_last(list_t *self) {
	return list_remove(self, list_end(self));
}

void *item_get(list_iterator_t *self) {
	return self->value;
}

void *item_set(list_iterator_t *self, void *value) {
	return (self->value = value);
}

list_iterator_t *item_next(list_iterator_t *self) {
	return self->next;
}

list_iterator_t *item_previous(list_iterator_t *self) {
	return self->previous;
}








