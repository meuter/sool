#include <test.h>
#include <cobject/list.h>

void test_constructor() {

}

int main() {
	unit_test_t all_tests[] = {
		unit_test(test_constructor),
	};
	return run_tests(all_tests);
}
