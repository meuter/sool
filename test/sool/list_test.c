#include <test.h>
#include <sool/list.h>
#include <sool/io.h>

// TODO check for memory leaks in the unit tests of list (use memwatch??)

static void test_empty_list() {
	list_t *empty = list();

	assert_false(empty == NULL);
	assert_true(list_is_empty(empty));
	assert_true(list_begin(empty) == list_end(empty));
	assert_true(list_rbegin(empty) == list_rend(empty));
	assert_int_equal(list_length(empty), 0);

	delete(empty);
}

static void test_list_int() {
	list_t *l = list(0,1,2,3,4,5,6,7,8,9);
	item_t *i;
	int j;

	assert_false(l == NULL);
	assert_false(list_is_empty(l));
	assert_true(list_end(l) == list_rend(l));

	// check traversal
	for (i = list_begin(l), j = 0; i != list_end(l); i = item_next(i), j++)
		assert_int_equal((int)item_get(i), j);

	// check traversal in reverse order
	for (i = list_rbegin(l), j = 9; i != list_rend(l); i = item_previous(i), j--)
		assert_int_equal((int)item_get(i), j);

	delete(l);
}


static void test_list_equal() {
	list_t *l1 = list(1,2,3,4);
	list_t *l2 = list(1,2,3,4);
	list_t *l3 = list(1,2,3);
	list_t *l4 = list(3,2,1);

	assert_false(equal(l1,l3));
	assert_true(equal(l1,l1));
	assert_true(equal(l1,l2));
	assert_false(equal(l3,l4));

	delete(l1,l2,l3,l4);
}

static void test_list_clone() {
	list_t *orig = list(1,2,3,4,5,6,7,8,9);
	list_t *clone = list_clone(orig);
	item_t *i;
	int j = 1;

	assert_false(orig == NULL);
	assert_false(orig == clone);
	assert_true(equal(orig, clone));

	delete(orig);

	assert_int_equal(list_length(clone), 9);
	list_forall(i, clone)
		assert_int_equal((int)item_get(i), j++);

	delete(clone);
}

static void test_list_clone_empty() {
	list_t *orig = list();
	list_t *clone = list_clone(orig);

	assert_false(orig == NULL);
	assert_false(orig == clone);
	assert_true(equal(orig, clone));

	delete(orig);

	assert_int_equal(list_length(clone), 0);
	assert_true(list_is_empty(clone));

	delete(clone);
}


void assert_list_equal(list_t *l1, list_t *l2) {
	assert_true(equal(l1,l2));
	delete(l1, l2);
}

static void test_list_slice() {
	list_t *orig = list(1,2,3,4,5,6,7,8,9);

	assert_list_equal(list_slice(orig,0,2), list(1,2));
	assert_list_equal(list_slice(orig,1,3), list(2,3));
	assert_list_equal(list_slice(orig,1,-1), list(2,3,4,5,6,7,8));
	assert_list_equal(list_slice(orig,0,10), list(1,2,3,4,5,6,7,8,9));

	delete(orig);
}

static void test_list_get() {
	list_t *l = list(0,1,2,3,4,5,6,7,8,9);
	int j;

	// check random access
	for (j = 0; j < 10; j++)
		assert_int_equal((int)item_get(list_get(l,j)), j);
	assert_true(list_get(l,10) == list_end(l));

	// check random access from the end
	for (j = -1; j >= -10; j--)
		assert_int_equal((int)item_get(list_get(l,j)), j+10);
	assert_true(list_get(l,-11) == list_end(l));

	assert_int_equal((int)item_get(list_get(l,-1)), 9);

	delete(l);
}

static void test_list_clear() {
	list_t *l = list(1,2,3);

	list_clear(l);
	assert_true(list_is_empty(l));
	assert_int_equal(list_length(l), 0);
	assert_true(list_begin(l) == list_end(l));
	assert_true(list_rbegin(l) == list_rend(l));

	delete(l);
}

static void test_list_find() {
	list_t *l = list(1,2,3,4);
	int i;

	assert_true(list_find(l, 0) == list_end(l));
	for (i = 1; i <=4; ++i) {
		assert_false(list_find(l,(void *)i) == list_end(l));
		assert_int_equal((int)item_get(list_find(l, (void *)i)), i);
	}

	delete(l);
}

static void test_list_rfind() {
	list_t *l = list(0,1,2,3,4,5,6,7,8,9,1,10);

	assert_true(list_rfind(l,(void *)11) == list_rend(l));
	assert_false(list_rfind(l,(void *)1) == list_rend(l));
	assert_false(list_find(l,(void *)1) == list_rfind(l,(void*)1));
	assert_true(list_find(l,(void *)5) == list_rfind(l,(void*)5));

	assert_true(item_next(list_find(l,(void *)1)) == list_find(l,(void*)2));
	assert_true(item_previous(list_find(l,(void *)1)) == list_find(l,(void*)0));

	assert_true(item_next(list_rfind(l,(void *)1)) == list_rfind(l,(void*)10));
	assert_true(item_previous(list_rfind(l,(void *)1)) == list_rfind(l,(void*)9));

	delete(l);
}

static void test_list_reverse() {
	list_t *l1 = list(1,2,3,4);
	list_t *l2 = list();

	assert_list_equal(list_reverse(l1), list(4,3,2,1));
	assert_list_equal(list_reverse(l2), list());

	delete(l1, l2);
}

static void test_list_join() {
	list_t *l1 = list(1,2,3,4);
	list_t *l2 = list();
	list_t *l3 = list(1,2,3);

	assert_list_equal(list_join(), list());
	assert_list_equal(list_join(l2), list());
	assert_list_equal(list_join(l2,l2), list());
	assert_list_equal(list_join(l2,l2,l2), list());
	assert_list_equal(list_join(l1,l2), list_clone(l1));
	assert_list_equal(list_join(l2,l1), list_clone(l1));
	assert_list_equal(list_join(l1,l1), list(1,2,3,4,1,2,3,4));
	assert_list_equal(list_join(l1,l1,l1), list(1,2,3,4,1,2,3,4,1,2,3,4));
	assert_list_equal(list_join(l1,l3,l1), list(1,2,3,4,1,2,3,1,2,3,4));

	delete(l1,l2,l3);
}

static void test_list_count() {
	list_t *l1 = list(1,2,3,2,1,2,4,1,1,4);
	list_t *l2 = list();

	assert_int_equal(list_count(l1,(void *)0), 0);
	assert_int_equal(list_count(l1,(void *)1), 4);
	assert_int_equal(list_count(l1,(void *)2), 3);
	assert_int_equal(list_count(l1,(void *)3), 1);
	assert_int_equal(list_count(l1,(void *)4), 2);
	assert_int_equal(list_count(l1,(void *)5), 0);

	assert_int_equal(list_count(l2,(void *)0), 0);
	assert_int_equal(list_count(l2,(void *)1), 0);
	assert_int_equal(list_count(l2,(void *)2), 0);
	assert_int_equal(list_count(l2,(void *)3), 0);
	assert_int_equal(list_count(l2,(void *)4), 0);
	assert_int_equal(list_count(l2,(void *)5), 0);

	delete(l1, l2);
}

static void test_list_sort() {
	list_t *l1 = list();
	list_t *l2 = list(1);
	list_t *l3 = list(1,2,3,4);
	list_t *l4 = list(3,2,5,1);
	list_t *l5 = list(1,5,6,9,3,2,9,5,2,3,6,8,7,4,8,5,9,6,3,2,1,5,2,5,6,3,5,8,7,4);

	assert_list_equal(list_sort(l1), list());
	assert_list_equal(list_sort(l2), list(1));
	assert_list_equal(list_sort(l3), list(1,2,3,4));
	assert_list_equal(list_sort(l4), list(1,2,3,5));
	assert_list_equal(list_sort(l5), list(1,1,2,2,2,2,3,3,3,3,4,4,5,5,5,5,5,5,6,6,6,6,7,7,8,8,8,9,9,9));

	delete(l1);
}

int main() {
	// TODO fix broken tests
	unit_test_t all_tests[] = {
		unit_test(test_empty_list),
		unit_test(test_list_int),
		unit_test(test_list_equal),
		unit_test(test_list_clone),
		unit_test(test_list_clone_empty),
		unit_test(test_list_slice),
		unit_test(test_list_get),
		unit_test(test_list_clear),
		unit_test(test_list_find),
		unit_test(test_list_rfind),
		unit_test(test_list_reverse),
		unit_test(test_list_join),
		unit_test(test_list_count),
		unit_test(test_list_sort),
	};
	return run_tests(all_tests);
}
