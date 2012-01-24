#include <test.h>

#include <sool/io.h>
#include <sool/string.h>

static void test_constructor() {
	string_t *s = string("hello world");
	string_t *t = string("");
	string_t *u = new(String(), 5, "0123456789");

	assert_int_equal(string_length(s), 11);
	assert_string_equal(string_cstr(s), "hello world");

	assert_int_equal(string_length(t), 0);
	assert_string_equal(string_cstr(t), "");

	assert_int_equal(string_length(u), 5);
	assert_string_equal(string_cstr(u), "01234");

	delete(s, t, u);
}

static void test_clone() {
	string_t *s = string("toto");
	string_t *t = string_clone(s);

	assert_true(equal(s, t));

	delete(s);

	assert_string_equal(string_cstr(t), "toto");

	delete(t);
}


static void test_find() {
	string_t *s = string("hello world"), *t = string("lo");
	string_t *u = string("foo"), *v = string("he"), *w = string("rld");

	assert_int_equal(string_find(s, u), -1);
	assert_int_equal(string_find(s, t), 3);
	assert_int_equal(string_find(s, v), 0);
	assert_int_equal(string_find(s, w), 8);

	delete(s, t, u, v, w);
}

static void test_set_get() {
	string_t *s = string("abcdefghijklmnopqrstuvwxyz");

	assert_true(string_get(s, 0) == 'a');
	assert_true(string_get(s, 25) == 'z');

	assert_true(string_get(s, -1) == 'z');
	assert_true(string_get(s, -26) == 'a');

	assert_true(string_set(s, 2, 'X') == 'X');
	assert_true(string_get(s, 2) == 'X');

	assert_true(string_set(s, -2, 'Y') == 'Y');
	assert_true(string_get(s, -2) == 'Y');

	assert_string_equal(string_cstr(s), "abXdefghijklmnopqrstuvwxYz");

	delete(s);
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_constructor),
//		unit_test(test_clone),
//		unit_test(test_find),
//		unit_test(test_set_get),
	};

	return run_tests(all_tests);
}
