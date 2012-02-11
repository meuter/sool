#include <test.h>

#include <sool/io.h>
#include <sool/string.h>
#include <sool/exception.h>

static void test_string_clone() {
	char *s = "hello", *x = string_clone(s);
	assert_string_equal(x, "hello");
	x[2] = 'c';
	assert_string_equal(x, "heclo");
	assert_string_equal(s, "hello");
	assert_raises(NullPointerError(), string_clone(NULL));
	delete(x);
}

static void test_string_length() {
	assert_int_equal(string_length(""), 0);
	assert_int_equal(string_length("hello"), 5);
	assert_int_equal(string_length("foo\0bar"), 3);
	assert_raises(NullPointerError(), string_length(NULL));
}

static void test_string_get() {
	assert_raises(NullPointerError(), string_get(NULL, 0));

	assert_raises(IndexError(), string_get("", 1));
	assert_raises(IndexError(), string_get("", -1));

	assert_true(string_get("", 0) == 0);
	assert_true(string_get("x", 0) == 'x');
	assert_true(string_get("x", -1) == 'x');
	assert_raises(IndexError(), string_get("x", 2));
	assert_raises(IndexError(), string_get("x", -2));

	assert_true(string_get("0123456789", 0) == '0');
	assert_true(string_get("0123456789", 4) == '4');
	assert_true(string_get("0123456789", 9) == '9');
	assert_true(string_get("0123456789", -1) == '9');
	assert_true(string_get("0123456789", -6) == '4');
	assert_true(string_get("0123456789", -10) == '0');
	assert_raises(IndexError(), string_get("0123456789", 11));
	assert_raises(IndexError(), string_get("0123456789", -11));
}

static void test_string_find() {
	const char *s = "hello world", *t = "lo";
	const char *u = "foo", *v = "he", *w = "rld";
	const char *y = "yoyo", *z = "yo";

	assert_int_equal(string_find(s, u), -1);
	assert_int_equal(string_find(s, t), 3);
	assert_int_equal(string_find(s, v), 0);
	assert_int_equal(string_find(s, w), 8);
	assert_int_equal(string_find(y, z), 0);

	assert_raises(NullPointerError(), string_find(s, NULL));
	assert_raises(NullPointerError(), string_find(NULL, s));
}


static void test_string_strip() {
	char *s, *t, *u, *v, *w, *x;

	assert_raises(NullPointerError(), string_strip(NULL));

	s = string_strip(""); assert_string_equal(s,"");
	t = string_strip("abcd"); assert_string_equal(t,"abcd");
	u = string_strip("   abcd"); assert_string_equal(u,"abcd");
	v = string_strip("abcd   "); assert_string_equal(v,"abcd");
	w = string_strip("\t \tabcd\n \n" ); assert_string_equal(w,"abcd");
	x = string_strip("\t \tab \ncd\n \n" ); assert_string_equal(x,"ab \ncd");

	delete(s, t, u, v, x);
}

// TODO rfind

// TODO contains

// TODO strip

// TODO split

// TODO join

static void test_string_slice() {
	const char *s = "0123456789", *t, *u, *v, *w, *x, *y, *z;

	assert_raises(NullPointerError(), string_slice(NULL, 0,0));
	assert_raises(IndexError(), string_slice(s, 0, 11));
	assert_raises(IndexError(), string_slice(s, 11, 2));
	assert_raises(IndexError(), string_slice(s, 0, -11));
	assert_raises(IndexError(), string_slice(s, -11, 2));

	t = string_slice(s, 0, 0);  assert_string_equal(t, "");
	u = string_slice(s, 0, 2);  assert_string_equal(u, "01");
	v = string_slice(s, 0, -1); assert_string_equal(v, "012345678");
	w = string_slice(s, -5, -2); assert_string_equal(w, "567");
	x = string_slice(s, 0, string_length(s)); assert_string_equal(x, s);
	y = string_slice(s, 5, 2);  assert_string_equal(y, "");
	z = string_slice(s, -2, -5);  assert_string_equal(z, "");

	delete(t, u, v, w, x, y, z);
}

// TODO replace

// TODO cmp

// TODO equal

static void test_string_ljust() {
	char *s;

	s = string_ljust("", 0, '-'); assert_string_equal(s,""); delete(s);
	s = string_ljust("", 5, '-'); assert_string_equal(s,"-----"); delete(s);
	s = string_ljust("abcd", 4, '-'); assert_string_equal(s,"abcd"); delete(s);
	s = string_ljust("abc", 4, '-'); assert_string_equal(s,"abc-"); delete(s);
	s = string_ljust("abc", 5, '-'); assert_string_equal(s,"abc--"); delete(s);
	s = string_ljust("abc", 6, '-'); assert_string_equal(s,"abc---"); delete(s);
	s = string_ljust("abc", 2, '-'); assert_string_equal(s,"abc"); delete(s);

	assert_raises(NullPointerError(), string_ljust(NULL, 3, '-'));
}

static void test_string_rjust() {
	char *s;

	s = string_rjust("", 0, '-'); assert_string_equal(s,""); delete(s);
	s = string_rjust("", 5, '-'); assert_string_equal(s,"-----"); delete(s);
	s = string_rjust("abcd", 4, '-'); assert_string_equal(s,"abcd"); delete(s);
	s = string_rjust("abc", 4, '-'); assert_string_equal(s,"-abc"); delete(s);
	s = string_rjust("abc", 5, '-'); assert_string_equal(s,"--abc"); delete(s);
	s = string_rjust("abc", 6, '-'); assert_string_equal(s,"---abc"); delete(s);
	s = string_rjust("abc", 2, '-'); assert_string_equal(s,"abc"); delete(s);

	assert_raises(NullPointerError(), string_rjust(NULL, 3, '-'));
}

static void test_string_center() {
	char *s;

	s = string_center("", 0, '-'); assert_string_equal(s,""); delete(s);
	s = string_center("", 5, '-'); assert_string_equal(s,"-----"); delete(s);
	s = string_center("abcd", 4, '-'); assert_string_equal(s,"abcd"); delete(s);
	s = string_center("abc", 4, '-'); assert_string_equal(s,"abc-"); delete(s);
	s = string_center("abc", 5, '-'); assert_string_equal(s,"-abc-"); delete(s);
	s = string_center("abc", 6, '-'); assert_string_equal(s,"-abc--"); delete(s);
	s = string_center("abc", 2, '-'); assert_string_equal(s,"abc"); delete(s);

	assert_raises(NullPointerError(), string_center(NULL, 3, '-'));
}

//TODO char   *string_lower       (const char *self);
//TODO char   *string_upper       (const char *self);
//TODO char   *string_title       (const char *self);
//TODO bool_t  string_starts_with (const char *self, const char *substr);
//TODO bool_t  string_ends_with   (const char *self, const char *substr);


static void test_string_format() {
	char *s = string_format("%08X", 10);

	assert_string_equal(s, "0000000A");
	assert_raises(NullPointerError(), string_format(NULL));

	delete(s);
}


int main() {
	unit_test_t all_tests[] = {
		unit_test(test_string_length),
		unit_test(test_string_clone),
		unit_test(test_string_get),
		unit_test(test_string_find),
		unit_test(test_string_strip),
		unit_test(test_string_slice),
		unit_test(test_string_ljust),
		unit_test(test_string_rjust),
		unit_test(test_string_center),
		unit_test(test_string_format),

	};

	return run_tests(all_tests);
}
