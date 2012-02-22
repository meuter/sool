#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmockery.h>
#include <stdbool.h>
#include <sool/exception.h>

typedef UnitTest unit_test_t;

#define assert_raises(exception, block) 												\
	do {																				\
		bool thrown = true, caught = false;											\
		try { block; thrown = false; } 													\
		catch(exception) { caught = true;	} 											\
		except { }																		\
		_assert_true(caught && thrown, "exception '" #exception "' not raised", __FILE__, __LINE__); 	\
	} while(0);

