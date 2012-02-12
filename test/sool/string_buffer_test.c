#include <test.h>
#include <sool/string_buffer.h>
#include <sool/string.h>

static void test_string_buffer_ctor() {
	string_buffer_t *buffer = new(StringBuffer());
	assert_true(cast(StringBuffer(), buffer) == buffer);
	assert_true(class_of(buffer) == StringBuffer());
	assert_int_equal(string_buffer_length(buffer), 0);
	assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_string_equal(string_buffer_value(buffer), "");
	delete(buffer);
}

static void test_string_buffer_append() {
	string_buffer_t *buffer = new(StringBuffer());
	assert_int_equal(string_buffer_append(buffer, "hello world"), 11);
	assert_string_equal(string_buffer_value(buffer), "hello world");
	assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_int_equal(string_buffer_length(buffer), 11);
	delete(buffer);
}

static void test_string_buffer_append_with_format() {
	string_buffer_t *buffer = new(StringBuffer());
	assert_int_equal(string_buffer_append(buffer, "%s", "hello world"), 11);
	assert_int_equal(string_buffer_append(buffer, "%d", 1515), 4);
	assert_string_equal(string_buffer_value(buffer), "hello world1515");
	assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_int_equal(string_buffer_length(buffer), 15);
	delete(buffer);
}

static void test_string_buffer_append_grow() {
	string_buffer_t *buffer = new(StringBuffer());
	int i;

	for (i = 0; i < STRING_BUFFER_DEFAULT_SIZE-1; ++i) {
		assert_int_equal(string_buffer_length(buffer), i);
		assert_int_equal(string_buffer_append(buffer, "X"), 1);
		assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	}

	printf("'%s'\n", string_buffer_value(buffer));
	assert_int_equal(string_buffer_length(buffer), i);
	assert_int_equal(string_buffer_append(buffer, "X"), 1);
	assert_int_equal(string_buffer_length(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_int_equal(string_buffer_size(buffer), (STRING_BUFFER_DEFAULT_SIZE+1) * 2);

	char *s = string_buffer_value(buffer);
	assert_int_equal(string_length(s), STRING_BUFFER_DEFAULT_SIZE);

	delete(buffer);
}

static void test_string_buffer_reset() {
	string_buffer_t *buffer = new(StringBuffer());
	assert_int_equal(string_buffer_append(buffer, "hello world"), 11);
	assert_string_equal(string_buffer_value(buffer), "hello world");
	assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_int_equal(string_buffer_length(buffer), 11);
	string_buffer_reset(buffer);
	assert_int_equal(string_buffer_length(buffer), 0);
	assert_int_equal(string_buffer_size(buffer), STRING_BUFFER_DEFAULT_SIZE);
	assert_string_equal(string_buffer_value(buffer), "");
	delete(buffer);
}


static void test_string_buffer_detach() {
	string_buffer_t *buffer = new(StringBuffer());
	assert_int_equal(string_buffer_append(buffer, "hello world"), 11);
	char *s = string_buffer_value(buffer);
	assert_string_equal(s, "hello world");
	string_buffer_detach(buffer);
	assert_int_equal(string_buffer_append(buffer, "hello world"), 11);
	assert_int_equal(string_buffer_length(buffer), 11);
	assert_int_equal(string_buffer_size(buffer), (((((0+1)*2)+1)*2)+1)*2);
	delete(buffer);
	assert_string_equal(s, "hello world");
	delete(s);
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_string_buffer_ctor),
		unit_test(test_string_buffer_append),
		unit_test(test_string_buffer_append_with_format),
		unit_test(test_string_buffer_append_grow),
		unit_test(test_string_buffer_reset),
		unit_test(test_string_buffer_detach),
	};

	return run_tests(all_tests);
}
