#include <sool/stack.h>
#include <test.h>


static void test_constructor() {
	stack_t *stack = new(Stack());
	delete(stack);
}

static void test_methods() {
	stack_t *stack = new(Stack());

	assert_true(stack_is_empty(stack));

	stack_push(stack, "toto");
	assert_false(stack_is_empty(stack));
	assert_string_equal(stack_top(stack), "toto");

	stack_push(stack, "foo");
	assert_false(stack_is_empty(stack));
	assert_string_equal(stack_top(stack), "foo");

	stack_push(stack, "yellow");
	assert_false(stack_is_empty(stack));
	assert_string_equal(stack_top(stack), "yellow");

	assert_string_equal(stack_pop(stack), "yellow");
	assert_false(stack_is_empty(stack));
	assert_string_equal(stack_pop(stack), "foo");
	assert_false(stack_is_empty(stack));

	stack_push(stack, "X");
	stack_push(stack, "X");
	stack_push(stack, "X");
	stack_push(stack, "X");
	stack_push(stack, "X");
	assert_false(stack_is_empty(stack));

	stack_clear(stack);
	assert_true(stack_is_empty(stack));
	delete(stack);
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_constructor),
		unit_test(test_methods),
	};

	return run_tests(all_tests);
}
