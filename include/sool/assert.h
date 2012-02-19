#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdbool.h>
#include <sool/exception.h>
#include <sool/string.h>

class_t *AssertionError();

#if 0

#define assert_true(expression)   _assert_true(expression, #expression)
#define assert_false(expression)  _assert_true(!expression, #expression)
#define assert_string_equal(s,t)  _assert_true(string_equal(s,t), #s " == " #t)
#define assert_int_equal(i, j)    _assert_true(i == j, #i " == " #j)
#define assert_raises(exception, block) 												\
	do {																				\
		bool_t thrown = TRUE, caught = FALSE;											\
		exception_t *e;																	\
		try { block; thrown = FALSE; } 													\
		catch(exception,e) { caught = TRUE;	} 											\
		except { }																		\
		_assert_true(caught && thrown, "exception '" #exception "' not raised" ); 	    \
	} while(0);

void _assert_true(bool_t expression, const char *string_expression);


#endif


#endif
