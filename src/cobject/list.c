#include <cobject/list.h>
#include <cobject/io.h>

struct _item_t {
	void *element;
	item_t next, previous;
};

struct _list_t {
	const class_t *class;
	item_t dummy;
	int length;
};


static void list_ctor(void *_self, va_list *args) {
    list_t self = _self;
    int i, n;

    self->dummy = malloc(sizeof(item_t));
    self->dummy->next = self->dummy->previous = self->dummy;
    self->length = 0;

    n = va_arg(*args, int);
    for (i = 0; i < n; ++i)
    	list_append(self, va_arg(*args, void *));
}

static void list_dtor(void *_self) {
	list_t self = _self;
	while (!list_is_empty(self))
		list_remove_first(self);
	free(self->dummy);
}

static int list_put(void *_self, FILE *stream, const char *format) {
	int result = 0;
	list_t self = _self;
	item_t i;

	if (format == NULL)
		format = "%p";

	result += fprintf(stream, "[");
	for (i = list_begin(self); i != list_end(self); i = item_next(i)) {
		result += ofprintf(stream, format, i->element);
		result += fprintf(stream, i->next != self->dummy ? ", " : "");
	}
	result += fprintf(stream, "]");
	return result;
}

static bool_t list_equal(void *_self, void *_other) {
	list_t self = _self, other = _other;
	item_t i, j;

	if (self == other)
		return TRUE;
	if (list_length(self) != list_length(other))
		return FALSE;

	for (i = list_begin(self), j = list_begin(other); i != list_end(self); i = item_next(i), j = item_next(j)) {
		if (item_get(i) != item_get(j))
			return FALSE;
	}

	return TRUE;
}


const class_t *List() {
	static const class_t *result = NULL;
	return result ? result : (result = class(sizeof(list_t), __FUNCTION__, list_ctor, list_dtor, list_put, list_equal));
}

list_t list_clone(list_t self) {
	return list_copy(self, list_begin(self), list_end(self));
}

list_t list_copy(list_t self, item_t from, item_t to) {
	(void)self;
	list_t result = list();
	item_t i = from;
	while (i != to) {
		list_append(result, item_get(i));
		i = item_next(i);
	}

	return result;
}

list_t list_slice(list_t self, int from, int to) {
	return list_copy(self, list_get(self, from), list_get(self, to));
}

//list_t list_join(list_t self, ...) {
//#warning not implemented
//	return NULL;
//}
//
//list_t list_reverse(list_t self) {
//#warning not implemented
//	return NULL;
//}
//
//list_t list_sort(list_t self) {
//#warning not implemented
//	return NULL;
//}

bool_t list_is_empty(list_t self) {
	return (self->length == 0);
}

//int list_count(list_t self, void *element) {
//#warning not implemented
//	return 0;
//}


void list_clear(list_t self) {
	while (!list_is_empty(self))
		list_remove_first(self);
}


int list_length(list_t self) {
	return self->length;
}

item_t list_get(list_t self, int i) {
	item_t j;

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

//item_t list_find(list_t self, void *element) {
//#warning not implemented
//	return NULL;
//}
//
//item_t list_prepend(list_t self) {
//#warning not implemented
//	return NULL;
//}

item_t list_append(list_t self, void *element) {
	item_t new_item = malloc(sizeof(item_t));

	new_item->previous = self->dummy->previous;
	new_item->previous->next = new_item;
	new_item->element = element;
	new_item->next = self->dummy;
	new_item->next->previous = new_item;

	self->length++;
	return new_item;
}


void *list_remove(list_t self, item_t to_remove) {
	to_remove->previous->next = to_remove->next;
	to_remove->next->previous = to_remove->previous;
	void *result = to_remove->element;
	free(to_remove);
	self->length--;
	return result;
}

item_t list_begin(list_t self) {
	return self->dummy->next;
}

item_t list_end(list_t self) {
	return self->dummy;
}

item_t list_rbegin(list_t self) {
	return self->dummy->previous;
}

item_t list_rend(list_t self) {
	return self->dummy;
}

void *list_remove_first(list_t self) {
	return list_remove(self, list_begin(self));
}

void *list_remove_last(list_t self) {
	return list_remove(self, list_end(self));
}

void *item_get(item_t self) {
	return self->element;
}

item_t item_next(item_t self) {
	return self->next;
}

item_t item_previous(item_t self) {
	return self->previous;
}








