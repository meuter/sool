#include <sool/assert.h>

#include "exception_def.h"

DEFINE_EXCEPTION(AssertionError);

//void _assert_true(bool_t expression, const char *string_expression) {
//	if (!expression)
//		throw(new(AssertionError(), string_expression));
//}
