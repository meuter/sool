#include <test.h>
#include <cobject/list.h>
#include <cobject/io.h>

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


static void test_list_equal() {
	list_t l1 = list(1,2,3,4);
	list_t l2 = list(1,2,3,4);
	list_t l3 = list(1,2,3);
	list_t l4 = list(3,2,1);

	assert_false(equal(l1, l3));
	assert_true(equal(l1, l1));
	assert_true(equal(l1, l2));
	assert_false(equal(l3, l4));

	delete(l1);
	delete(l2);
	delete(l3);
	delete(l4);

}

static void test_list_clone() {
	list_t orig = list(1,2,3,4,5,6,7,8,9);
	list_t clone = list_clone(orig);
	item_t i;
	int j = 1;

	assert_false(orig == NULL);
	assert_false(orig == clone);
	assert_true(equal(orig, clone));

	delete(orig);

	assert_int_equal(list_length(clone), 9);
	list_forall(i, orig)
		assert_int_equal((int)item_get(i), j++);

	delete(clone);
}

static void test_list_clone_empty() {
	list_t orig = list();
	list_t clone = list_clone(orig);

	assert_false(orig == NULL);
	assert_false(orig == clone);
	assert_true(equal(orig, clone));

	delete(orig);

	assert_int_equal(list_length(clone), 0);
	assert_true(list_is_empty(clone));

	delete(clone);
}


#define assert_list_equal(l1, l2) do {\
		assert_true(equal(l1,l2));\
		delete(l1);\
		delete(l2);\
	} while(0);

static void test_list_slice() {
	list_t orig = list(1,2,3,4,5,6,7,8,9);

	assert_list_equal(list_slice(orig,0,2), list(1,2));
	assert_list_equal(list_slice(orig,1,3), list(2,3));
	assert_list_equal(list_slice(orig,1,-1), list(2,3,4,5,6,7,8));
	assert_list_equal(list_slice(orig,0,10), list(1,2,3,4,5,6,7,8,9));

	delete(orig);
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

	assert_int_equal((int)item_get(list_get(l,-1)), 9);

	delete(l);
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_empty_list),
		unit_test(test_list_int),
		unit_test(test_list_equal),
		unit_test(test_list_clone),
		unit_test(test_list_clone_empty),
		unit_test(test_list_slice),
		unit_test(test_list_get),
	};
	return run_tests(all_tests);
}
