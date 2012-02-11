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

	s = string_clone(""); assert_string_equal(s,""); delete(s);

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
	assert_int_equal(string_find("hello world", "foo"), -1);
	assert_int_equal(string_find("hello world", "lo"), 3);
	assert_int_equal(string_find("hello world", "he"), 0);
	assert_int_equal(string_find("hello world", "rld"), 8);
	assert_int_equal(string_find("yoyo", "yo"), 0);
	assert_int_equal(string_find("toyoyo", "yo"), 2);

	assert_raises(NullPointerError(), string_find(NULL, NULL));
	assert_raises(NullPointerError(), string_find("", NULL));
	assert_raises(NullPointerError(), string_find(NULL, ""));
}

static void test_string_contains() {
	assert_false(string_contains("hello world", "foo"));
	assert_true(string_contains("hello", "hello"));
	assert_true(string_contains("hello world", "lo"));
	assert_true(string_contains("hello world", "he"));
	assert_true(string_contains("hello world", "rld"));
	assert_true(string_contains("yoyo", "yo"));
	assert_true(string_contains("toyoyo", "yo"));

	assert_raises(NullPointerError(), string_contains(NULL, NULL));
	assert_raises(NullPointerError(), string_contains("", NULL));
	assert_raises(NullPointerError(), string_contains(NULL, ""));
}

static void test_string_strip() {
	char *s;

	assert_raises(NullPointerError(), string_strip(NULL));

	s = string_strip(""); assert_string_equal(s,""); delete(s);
	s = string_strip("abcd"); assert_string_equal(s,"abcd"); delete(s);
	s = string_strip("   abcd"); assert_string_equal(s,"abcd"); delete(s);
	s = string_strip("abcd   "); assert_string_equal(s,"abcd"); delete(s);
	s = string_strip("\t \tabcd\n \n" ); assert_string_equal(s,"abcd"); delete(s);
	s = string_strip("\t \tab \ncd\n \n" ); assert_string_equal(s,"ab \ncd"); delete(s);
}

// TODO rfind

static void test_string_split() {
	list_t *l;
	iterator_t *i;

	assert_raises(NullPointerError(), string_split(NULL, ""));
	assert_raises(NullPointerError(), string_split("", NULL));
	assert_raises(NullPointerError(), string_split(NULL, NULL));

	l = string_split("first second and third", ", ");
	assert_int_equal(list_length(l), 1);
	i = begin(l); assert_string_equal(get(i), "first second and third"); delete(get(i));
	delete(l);

	l = string_split("first second and third", " and ");
	assert_int_equal(list_length(l), 2);
	i = begin(l); assert_string_equal(get(i), "first second"); delete(get(i));
	i = next(i);  assert_string_equal(get(i), "third");        delete(get(i));
	delete(l);

	l = string_split("first second and third", " ");
	assert_int_equal(list_length(l), 4);
	i = begin(l); assert_string_equal(get(i), "first");  delete(get(i));
	i = next(i);  assert_string_equal(get(i), "second"); delete(get(i));
	i = next(i);  assert_string_equal(get(i), "and");    delete(get(i));
	i = next(i);  assert_string_equal(get(i), "third");  delete(get(i));
	delete(l);
}

static void test_string_join() {
	char *s;

	assert_raises(NullPointerError(), string_join(NULL));
	assert_raises(NullPointerError(), string_join(", ", NULL));
	assert_raises(NullPointerError(), string_join(", ", "first", NULL));

	s = string_join(""); assert_string_equal(s, ""); delete(s);
	s = string_join(", "); assert_string_equal(s, ""); delete(s);
	s = string_join(", "); assert_string_equal(s, ""); delete(s);
	s = string_join(", ", "first"); assert_string_equal(s, "first"); delete(s);
	s = string_join(", ", "first", "second"); assert_string_equal(s, "first, second"); delete(s);
	s = string_join("", "first", "second"); assert_string_equal(s, "firstsecond"); delete(s);
	s = string_join("/", "first", "second", "third"); assert_string_equal(s, "first/second/third"); delete(s);
	s = string_join("/", "", "second", ""); assert_string_equal(s, "/second/"); delete(s);
}

static void test_string_slice() {
	const char *s;

	assert_raises(NullPointerError(), string_slice(NULL, 0,0));
	assert_raises(IndexError(), string_slice("0123456789", 0, 11));
	assert_raises(IndexError(), string_slice("0123456789", 11, 2));
	assert_raises(IndexError(), string_slice("0123456789", 0, -11));
	assert_raises(IndexError(), string_slice("0123456789", -11, 2));

	s = string_slice("0123456789", 0, 0);  assert_string_equal(s, "");
	s = string_slice("0123456789", 0, 2);  assert_string_equal(s, "01");
	s = string_slice("0123456789", 0, -1); assert_string_equal(s, "012345678");
	s = string_slice("0123456789", -5, -2); assert_string_equal(s, "567");
	s = string_slice("0123456789", 0, 10); assert_string_equal(s, "0123456789");
	s = string_slice("0123456789", 5, 2);  assert_string_equal(s, "");
	s = string_slice("0123456789", -2, -5);  assert_string_equal(s, "");
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

static void test_string_upper() {
	char *s;

	assert_raises(NullPointerError(), string_upper(NULL));
	s = string_upper("1234"); assert_string_equal(s, "1234"); delete(s);
	s = string_upper("abcz"); assert_string_equal(s, "ABCZ"); delete(s);
	s = string_upper("abCddZ"); assert_string_equal(s, "ABCDDZ"); delete(s);
}

static void test_string_lower() {
	char *s;

	assert_raises(NullPointerError(), string_lower(NULL));
	s = string_lower("1234"); assert_string_equal(s, "1234"); delete(s);
	s = string_lower("ABCDZ"); assert_string_equal(s, "abcdz"); delete(s);
	s = string_lower("abCddZ"); assert_string_equal(s, "abcddz"); delete(s);
}

//TODO char   *string_title       (const char *self);

static void test_string_starts_with() {
	assert_raises(NullPointerError(), string_ends_with(NULL, NULL));
	assert_raises(NullPointerError(), string_ends_with("", NULL));
	assert_raises(NullPointerError(), string_ends_with(NULL, ""));

	assert_true(string_ends_with("", ""));
	assert_true(string_ends_with("abcd", "abcd"));
	assert_true(string_ends_with("abcd", "bcd"));
	assert_true(string_ends_with("abcd", "cd"));
	assert_true(string_ends_with("abcd", "d"));
	assert_true(string_ends_with("abcd", ""));

	assert_false(string_ends_with("abcd", "cabcd"));
	assert_false(string_ends_with("abcd", "cabc"));
	assert_false(string_ends_with("abcd", "cab"));
	assert_false(string_ends_with("abcd", "ca"));
	assert_false(string_ends_with("abcd", "c"));
}

static void test_string_ends_with() {
	assert_raises(NullPointerError(), string_starts_with(NULL, NULL));
	assert_raises(NullPointerError(), string_starts_with("", NULL));
	assert_raises(NullPointerError(), string_starts_with(NULL, ""));

	assert_true(string_starts_with("", ""));
	assert_true(string_starts_with("abcd", "abcd"));
	assert_true(string_starts_with("abcd", "abc"));
	assert_true(string_starts_with("abcd", "ab"));
	assert_true(string_starts_with("abcd", "a"));
	assert_true(string_starts_with("abcd", ""));

	assert_false(string_starts_with("abcd", "cabcd"));
	assert_false(string_starts_with("abcd", "cabc"));
	assert_false(string_starts_with("abcd", "cab"));
	assert_false(string_starts_with("abcd", "ca"));
	assert_false(string_starts_with("abcd", "c"));
}

static void test_string_format() {
	char *s = string_format("%08X", 10);
	assert_string_equal(s, "0000000A");
	assert_raises(NullPointerError(), string_format(NULL));
	delete(s);
}


static void test_string_is_space() {
	assert_raises(NullPointerError(), string_is_space(NULL));
	assert_false(string_is_space(""));
	assert_true(string_is_space(" "));
	assert_true(string_is_space("\n"));
	assert_true(string_is_space("\t"));
	assert_true(string_is_space(" \n \t"));
	assert_false(string_is_space("   x"));
}


static void test_string_is_lower() {
	assert_raises(NullPointerError(), string_is_lower(NULL));
	assert_false(string_is_lower(""));
	assert_false(string_is_lower(" "));
	assert_false(string_is_lower("aA"));
	assert_true(string_is_lower("a"));
	assert_true(string_is_lower(" a "));
}

static void test_string_is_upper() {
	assert_raises(NullPointerError(), string_is_upper(NULL));
	assert_false(string_is_upper(""));
	assert_false(string_is_upper(" "));
	assert_false(string_is_upper("aA"));
	assert_true(string_is_upper("A"));
	assert_true(string_is_upper(" A "));
}

static void test_string_is_alpha() {
	assert_raises(NullPointerError(), string_is_alpha(NULL));
	assert_false(string_is_alpha(""));
	assert_false(string_is_alpha(" "));
	assert_true(string_is_alpha("aA"));
	assert_false(string_is_alpha("a A"));
	assert_true(string_is_alpha("A"));
	assert_false(string_is_alpha(" A "));
	assert_true(string_is_alpha("a"));
	assert_false(string_is_alpha(" a "));
}

static void test_string_is_num() {
	assert_raises(NullPointerError(), string_is_num(NULL));
	assert_false(string_is_num(""));
	assert_false(string_is_num(" "));
	assert_true(string_is_num("12"));
	assert_false(string_is_num("1 2"));
	assert_true(string_is_num("1"));
	assert_false(string_is_num(" 2 "));
	assert_true(string_is_num("1"));
	assert_false(string_is_num(" 3 "));
}


static void test_string_is_alphanum() {
	assert_raises(NullPointerError(), string_is_alphanum(NULL));
	assert_false(string_is_alphanum(""));
	assert_false(string_is_alphanum(" "));
	assert_true(string_is_alphanum("aA"));
	assert_false(string_is_alphanum("a A"));
	assert_true(string_is_alphanum("A"));
	assert_false(string_is_alphanum(" A "));
	assert_true(string_is_alphanum("a"));
	assert_false(string_is_alphanum(" a "));
	assert_false(string_is_alphanum(" "));
	assert_true(string_is_alphanum("aA1"));
	assert_false(string_is_alphanum("a 1A"));
	assert_true(string_is_alphanum("a1"));
	assert_true(string_is_alphanum("21"));
}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_string_clone),
		unit_test(test_string_length),
		unit_test(test_string_get),
		unit_test(test_string_find),
		unit_test(test_string_contains),
		unit_test(test_string_strip),
		unit_test(test_string_split),
		unit_test(test_string_join),
		unit_test(test_string_slice),
		unit_test(test_string_ljust),
		unit_test(test_string_rjust),
		unit_test(test_string_center),
		unit_test(test_string_lower),
		unit_test(test_string_upper),
		unit_test(test_string_starts_with),
		unit_test(test_string_ends_with),
		unit_test(test_string_format),
		unit_test(test_string_is_space),
		unit_test(test_string_is_lower),
		unit_test(test_string_is_upper),
		unit_test(test_string_is_alpha),
		unit_test(test_string_is_num),
		unit_test(test_string_is_alphanum),
	};

	return run_tests(all_tests);
}
