#include <cobject/list.h>
#include <cobject/io.h>

struct _item_t {
	void *value;
	item_t *next, *previous;
};

struct _list_t {
	const class_t *class;
	item_t *dummy;
	int length;
};


static void list_ctor(void *_self, va_list *args) {
    list_t *self = _self;
    int i, n;

    self->dummy = malloc(sizeof(item_t));
    self->dummy->next = self->dummy->previous = self->dummy;
    self->length = 0;

    n = va_arg(*args, int);
    for (i = 0; i < n; ++i)
    	list_append(self, va_arg(*args, void *));
}

static void list_dtor(void *_self) {
	list_t *self = _self;
	while (!list_is_empty(self))
		list_remove_first(self);
	free(self->dummy);
}

static int list_put(void *_self, FILE *stream, const char *format) {
	int result = 0;
	list_t *self = _self;
	item_t *i;

	if (format == NULL)
		format = "%p";

	result += fprintf(stream, "[");
	for (i = list_begin(self); i != list_end(self); i = item_next(i)) {
		result += ofprintf(stream, format, i->value);
		result += fprintf(stream, i->next != self->dummy ? ", " : "");
	}
	result += fprintf(stream, "]");
	return result;
}

static bool_t list_equal(void *_self, void *_other) {
	list_t *self = _self, *other = _other;
	item_t *i, *j;

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

list_t *list_clone(list_t *self) {
	return list_copy(self, list_begin(self), list_end(self));
}

list_t *list_copy(list_t *self, item_t *from, item_t *to) {
	(void)self;
	list_t *result = list();
	item_t *i = from;
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
	item_t *i;
	va_list args;

	va_start(args, n);
	while (n--) {
		tmp = va_arg(args, list_t *);
		list_forall(i, tmp)
			list_append(result, item_get(i));
	}
	va_end(args);

	return result;
}



list_t *list_sort(list_t *self) {
	list_t *result = list();
	item_t *i, *j;

	list_forall(i, self) {
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
	item_t *i;
	list_forall(i, self)
		list_prepend(result, item_get(i));
	return result;
}

bool_t list_is_empty(list_t *self) {
	return (self->length == 0);
}

int list_count(list_t *self, void *value) {
	item_t *i;
	int result = 0;
	list_forall(i, self)
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

item_t *list_get(list_t *self, int i) {
	item_t *j;

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

void list_set(list_t *self, int i, void *value) {
	item_set(list_get(self, i), value);
}

item_t *list_find(list_t *self, void *value) {
	item_t *i;
	list_forall(i, self) {
		if (item_get(i) == value)
			break;
	}
	return i;
}

item_t *list_rfind(list_t *self, void *value) {
	item_t *i;
	for (i = list_rbegin(self); i != list_rend(self); i = item_previous(i)) {
		if (item_get(i) == value)
			break;
	}
	return i;
}

item_t *list_insert_before(list_t *self, item_t *item, void *value) {
	item_t *new_item = malloc(sizeof(struct _item_t));

	new_item->next = item->next;
	new_item->next->previous = new_item;
	new_item->value = value;
	new_item->previous = item;
	new_item->previous->next = new_item;

	self->length++;
	return new_item;
}


item_t *list_insert_after(list_t *self, item_t *item, void *value) {
	item_t *new_item = malloc(sizeof(struct _item_t));

	new_item->previous = item->previous;
	new_item->previous->next = new_item;
	new_item->value = value;
	new_item->next = item;
	new_item->next->previous = new_item;

	self->length++;
	return new_item;
}

item_t *list_prepend(list_t *self, void *value) {
	return list_insert_before(self, list_rend(self), value);
}

item_t *list_append(list_t *self, void *value) {
	return list_insert_after(self, list_end(self), value);
}

item_t *list_begin(list_t *self) {
	return self->dummy->next;
}

item_t *list_end(list_t *self) {
	return self->dummy;
}

item_t *list_rbegin(list_t *self) {
	return self->dummy->previous;
}

item_t *list_rend(list_t *self) {
	return self->dummy;
}


void *list_remove(list_t *self, item_t *to_remove) {
	to_remove->previous->next = to_remove->next;
	to_remove->next->previous = to_remove->previous;
	void *result = to_remove->value;
	free(to_remove);
	self->length--;
	return result;
}

void *list_remove_first(list_t *self) {
	return list_remove(self, list_begin(self));
}

void *list_remove_last(list_t *self) {
	return list_remove(self, list_end(self));
}

void *item_get(item_t *self) {
	return self->value;
}

void item_set(item_t *self, void *value) {
	self->value = value;
}

item_t *item_next(item_t *self) {
	return self->next;
}

item_t *item_previous(item_t *self) {
	return self->previous;
}








