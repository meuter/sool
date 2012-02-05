#include <test.h>
#include <sool/stack.h>
#include <sool/exception.h>

extern stack_t *stack_trace;

/*****************************************************************************/

void fatalf(char *error_message, ...) {
	check_expected(error_message);
}

void _assertf(int expression, char *error_message, ...) {
	(void)expression;
	(void)error_message;
}

/*****************************************************************************/

static void setup() {
}

static void teardown() {
	if (stack_trace != NULL) {
		while (!stack_is_empty(stack_trace))
			(void)stack_pop(stack_trace);
		delete(stack_trace);
		stack_trace = NULL;
	}
}

/*****************************************************************************/

static void test_throw_no_try() {
	expect_any(fatalf, error_message);
	assert_true(stack_trace == NULL);
	throw(Exception());
	assert_true(stack_trace == NULL);
}

static void test_try_no_catch() {
	bool_t passed = FALSE;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		passed = TRUE;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_try_throw_no_catch() {
	expect_any(fatalf, error_message);
	assert_true(stack_trace == NULL);
	try {
		throw(Exception());
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_nested_try_throw_no_catch() {
	assert_true(stack_trace == NULL);
	try {
		try { throw(Exception()); }
	}
	except {
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_try_no_throw_catch() {
	class_t *e;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
	}
	catch(Class(), e) {
		assert_true(FALSE);
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_try_no_throw_except() {
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
	}
	except {
		assert_true(FALSE);
	}
	assert_true(stack_is_empty(stack_trace));
}


static void test_try_throw_catch() {
	class_t *c;
	bool_t passed = FALSE;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		throw(Exception());
		assert_true(FALSE);
	}
	catch(Class(), c) {
		passed = TRUE;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_try_throw_except() {
	bool_t passed = FALSE;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		throw(Exception());
		assert_true(FALSE);
	}
	except {
		passed = TRUE;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}


int main() {
	unit_test_t all_tests[] = {
		unit_test_setup_teardown(test_throw_no_try, setup, teardown),
		unit_test_setup_teardown(test_try_no_catch, setup, teardown),
		unit_test_setup_teardown(test_try_throw_no_catch, setup, teardown),
		unit_test_setup_teardown(test_nested_try_throw_no_catch, setup, teardown),
		unit_test_setup_teardown(test_try_no_throw_except, setup, teardown),
		unit_test_setup_teardown(test_try_no_throw_catch, setup, teardown),
		unit_test_setup_teardown(test_try_throw_catch, setup, teardown),
		unit_test_setup_teardown(test_try_throw_except, setup, teardown),
	};

	return run_tests(all_tests);
}
