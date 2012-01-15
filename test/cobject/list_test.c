#include <test.h>
#include <cobject/list.h>

static void test_empty_list() {
	list_t empty = list();

	assert_false(empty == NULL);
	assert_true(list_is_empty(empty));
	assert_true(list_begin(empty) == list_end(empty));
	assert_true(list_rbegin(empty) == list_rend(empty));
	assert_int_equal(list_length(empty), 0);

	delete(empty);
}

static void test_list_int() {
	list_t l = list(0,1,2,3,4,5,6,7,8,9);
	item_t i;
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


static void test_list_get() {
	list_t l = list(0,1,2,3,4,5,6,7,8,9);
	int j;

	// check random access
	for (j = 0; j < 10; j++)
		assert_int_equal((int)item_get(list_get(l,j)), j);
	assert_true(list_get(l,10) == list_end(l));

	// check random access from the end
	for (j = -1; j >= -10; j--)
		assert_int_equal((int)item_get(list_get(l,j)), j+10);
	assert_true(list_get(l,-11) == list_end(l));

	delete(l);
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_empty_list),
		unit_test(test_list_int),
		unit_test(test_list_get),
	};
	return run_tests(all_tests);
}
