#include <cobject/list.h>
#include <cobject/io.h>

struct _item_t;
typedef struct _item_t item_t;

struct _item_t {
	void *element;
	item_t *next, *previous;
};

struct _list_t {
	const class_t *class;
	item_t *dummy;
	int length;
};

static void List_ctor(void *_self, va_list *args) {
    list_t *self = _self;
    int i, n;

    self->dummy = malloc(sizeof(item_t));
    self->dummy->next = self->dummy->previous = self->dummy;
    self->length = 0;

    n = va_arg(*args, int);
    for (i = 0; i < n; ++i)
    	list_append(self, va_arg(*args, void *));
}

static void List_dtor(void *_self) {
	list_t *self = _self;
	while (!list_is_empty(self))
		list_remove_first(self);
	free(self->dummy);
}

static int List_put(void *_self, FILE *stream, const char *format) {
	int result = 0;
	list_t *self = _self;
	item_t *item = self->dummy->next;

	if (format == NULL)
		format = "%p";

	result += fprintf(stream, "[");
	while (item != self->dummy) {
		result += ofprintf(stream, format, item->element);
		result += fprintf(stream, item->next != self->dummy ? ", " : "");
		item = item->next;
	}
	result += fprintf(stream, "]");
	return result;
}

const class_t *List() {
	static const class_t *result = NULL;
	return result ? result : class(sizeof(list_t), __FUNCTION__, List_ctor, List_dtor, List_put, NULL);
}

void list_append(list_t *self, void *element) {
	item_t *new_item = malloc(sizeof(item_t));

	new_item->previous = self->dummy->previous;
	new_item->previous->next = new_item;
	new_item->element = element;
	new_item->next = self->dummy;
	new_item->next->previous = new_item;

	self->length++;
}

bool_t list_is_empty(list_t *self) {
	return (self->length == 0);
}

int list_get_length(list_t *self) {
	return self->length;
}

static void *list_remove(list_t *self, item_t *to_remove) {
	to_remove->previous->next = to_remove->next;
	to_remove->next->previous = to_remove->previous;
	void *result = to_remove->element;
	free(to_remove);
	self->length--;
	return result;
}

void *list_remove_first(list_t *self) {
	return list_is_empty(self) ? NULL : list_remove(self, self->dummy->next);
}

void *list_remove_last(list_t *self) {
	return list_is_empty(self) ? NULL : list_remove(self, self->dummy->previous);
}







