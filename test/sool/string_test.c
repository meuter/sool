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

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_constructor),
		unit_test(test_clone),
	};

	return run_tests(all_tests);
}
