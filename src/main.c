#include <sool/assert.h>
#include <sool/io.h>
#include <sool/error.h>
#include <stdlib.h>
#include <stdbool.h>


#ifdef ALPHA
struct _unit_test_t {
	void (*test_function)();
	const char *test_name;
};

typedef struct _unit_test_t unit_test_t;

#define unit_test(function) { .test_function = function, .test_name = #function }


#define run_tests(v) _run_tests(v, sizeof(v)/sizeof(unit_test_t))


#define PASSED "\033[01;32m"
#define FAILED "\033[01;31m"
#define NORMAL "\033[00m"



int _run_tests(unit_test_t all_tests[], int n) {
	int i, result = 0;
	exception_t *e;

	file_print(stdout, "%s\n", string_center(" [ " __FILE__ " ] ", 80, '-'));
	for (i = 0; i < n; ++i) {
		try {
			file_print(stdout, "%s: starting test\n", all_tests[i].test_name);
			all_tests[i].test_function();
			file_print(stdout, "%s: test " PASSED "passed! " NORMAL "\n", all_tests[i].test_name);
			result += 1;
		}
		catch(AssertionError(), e) {
			file_print(stderr, FAILED "uncaught exception: %O\n", e);
			error_print_backtrace(stderr);
			file_print(stderr, NORMAL);
			file_print(stderr, "%s: test " FAILED "failed!" NORMAL "\n", all_tests[i].test_name);
		}
	}
	file_print(stdout, "%s\n", string_center("", 80, '-'));
	file_print(stdout, "%d / %d tests passed successfully.\n", n-result, n);
	file_print(stdout, "%s\n", string_center("", 80, '-'));


	return n-result;
}


void test_1() {
	assert_true(false);
}

void test_2() {
	assert_true(true);
}

void test_3() {
	try {
		assert_true(false);
	}
	except {}
}

void test_string() {
	assert_string_equal("hello", "foo");
}
#endif

int main(void) {

	exception_t *e = new(NullPointerError());

	println("NullPointerError is a Exception ? %b", is_a(Exception(), e));
	println("NullPointerError is a Class ? %b", is_a(Class(), e));
	println("NullPointerError is a Object ? %b", is_a(Object(), e));
	println("NullPointerError is a NullPointerError ? %b", is_a(NullPointerError(), e));
	println("NullPointerError is a IndexError ? %b", is_a(IndexError(), e));

#ifdef ALPHA
	unit_test_t all_tests[] = {
		unit_test(test_1),
		unit_test(test_2),
		unit_test(test_3),
		unit_test(test_string),
	};

	return run_tests(all_tests);
#endif

	return 0;
}



