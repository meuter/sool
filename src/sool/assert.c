#include <sool/assert.h>

#include "exception_def.h"

DEFINE_EXCEPTION(AssertionError);

#if 0
void _assert_true(bool expression, const char *string_expression) {
	if (!expression)
		throw(new(AssertionError(), string_expression));
}

#endif
