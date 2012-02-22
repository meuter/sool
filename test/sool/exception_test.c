#include <test.h>
#include <sool/stack.h>
#include <sool/exception.h>
#include <sool/io.h>

extern __thread stack_t *stack_trace;

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
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_try_throw_no_catch() {
	expect_any(fatalf, error_message);
	assert_true(stack_trace == NULL);
	try {
		throw(new(Exception(), ""));
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_nested_try_throw_no_catch() {
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		try { throw(Exception()); }
	}
	except {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_try_no_throw_catch() {
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
	}
	catch(Exception()) {
		assert_true(false);
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_try_no_throw_except() {
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
	}
	except {
		assert_true(false);
	}
	assert_true(stack_is_empty(stack_trace));
}


static void test_try_throw_catch() {
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		throw(new(Exception(), ""));
		assert_true(false);
	}
	catch(Exception()) {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_try_throw_except() {
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		throw(Exception());
		assert_true(false);
	}
	except {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}


static void test_nested_uncaught() {
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		try {
			assert_false(stack_is_empty(stack_trace));
			throw(new(Exception(), ""));
			assert_true(false);
		}
		assert_true(false);
	}
	except {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_nested_caught() {
	bool passed = false;
	assert_true(stack_trace == NULL);
	try {
		assert_false(stack_is_empty(stack_trace));
		try {
			assert_false(stack_is_empty(stack_trace));
			throw(new(Exception(), ""));
			assert_true(false);
		}
		except {
			passed = true;
		}
	}
	except {
		assert_true(false);
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_filtered() {
	bool passed = false;
	try {
		throw(new(Exception(),""));
		assert_true(false);
	}
	catch(NullPointerError()) {
		assert_true(false);
	}
	catch(Exception()) {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_throw_in_a_catch() {
	bool passed = false;
	try {
		try {
			throw(new(Exception(),""));
			assert_true(false);
		}
		catch(Exception()) {
			exception_t *e = exception_get();
			throw(e);
			assert_true(false);
		}
		assert_true(false);
	}
	except {
		passed = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(passed);
}

static void test_nested_empty_try() {

	try {
		try {
			try {
				assert_false(stack_is_empty(stack_trace));
			}
			assert_false(stack_is_empty(stack_trace));
		}
		assert_false(stack_is_empty(stack_trace));
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_nested_empty_try_one_catch() {
	try {
		try {
			try {

			}
		}
		catch (Exception()) {
			assert_true(false);
		}
	}
	assert_true(stack_is_empty(stack_trace));
}

static void test_nested_try_thrown_let_through_and_caught() {
	int step = 0;
	try {
		try {
			try {
				throw(new(Exception(), ""));
				assert_true(false);
			}
			assert_true(false);
		}
		catch(Exception()) {
			step += 1;
		}
		step += 1;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_int_equal(step, 2);
}

static void test_throw_uncaught_exception() {
	bool passed = false;
	try {
		try {
			throw(new(Exception(),""));
			assert_true(false);
		}
		catch(Exception()) {
			passed = true;
		}
		assert_false(stack_is_empty(stack_trace));
	}
	assert_true(passed);
}


static void sub_function_that_throws_an_exception() {
	throw(new(Exception(), ""));
}


static void test_throw_in_a_subfunction() {
	bool passed = false;
	try {
		sub_function_that_throws_an_exception();
		assert_true(false);
	}
	catch(Exception()) {
		passed = true;
	}
	assert_true(passed);
}


static void test_try_catch_finally() {
	bool caught = false, finalized = false;

	try {
		throw(new(Exception(), ""));
		assert_true(false);
	}
	catch(Exception()) {
		caught = true;
	}
	finally {
		finalized = true;
	}

	assert_true(caught && finalized);
}


static void test_try_catch_finally_no_throw() {
	bool caught = false, finalized = false, passed = false;

	try {
		passed = true;
	}
	catch(Exception()) {
		caught = true;
	}
	finally {
		finalized = true;
	}

	assert_true(passed && !caught && finalized);
}

static void test_throw_except_rethrow_finalized() {
	bool caught = false, finalized = false;
	try {
		try {
			throw(new(Exception(),""));
			assert_true(false);
		}
		except {
			rethrow();
			assert_true(false);
		}
		assert_true(false);
	}
	except {
		caught = true;
	}
	finally {
		finalized = true;
	}
	assert_true(stack_is_empty(stack_trace));
	assert_true(caught && finalized);
}

#define trace() println("%s:%d:%s (stack_size = %d)", __FILE__, __LINE__, __FUNCTION__, stack_trace == NULL? -1 : stack_size(stack_trace));

static void test_throw_except_rethrow_uncaught() {
	expect_any(fatalf, error_message);
	try {
		throw(new(Exception(),""));
		assert_true(false);
	}
	except {
		rethrow();
		return; // the real fatal stops
	}
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
		unit_test_setup_teardown(test_nested_uncaught, setup, teardown),
		unit_test_setup_teardown(test_nested_caught, setup, teardown),
		unit_test_setup_teardown(test_filtered, setup, teardown),
		unit_test_setup_teardown(test_throw_in_a_catch, setup, teardown),
		unit_test_setup_teardown(test_nested_empty_try, setup, teardown),
		unit_test_setup_teardown(test_nested_empty_try_one_catch, setup, teardown),
		unit_test_setup_teardown(test_nested_try_thrown_let_through_and_caught, setup, teardown),
		unit_test_setup_teardown(test_throw_uncaught_exception, setup, teardown),
		unit_test_setup_teardown(test_throw_in_a_subfunction, setup, teardown),
		unit_test_setup_teardown(test_try_catch_finally, setup, teardown),
		unit_test_setup_teardown(test_try_catch_finally_no_throw, setup, teardown),
		unit_test_setup_teardown(test_throw_except_rethrow_finalized, setup, teardown),
		unit_test_setup_teardown(test_throw_except_rethrow_uncaught, setup, teardown),
	};

	return run_tests(all_tests);
}
