#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmockery.h>
#include <sool/bool.h>
#include <sool/exception.h>

typedef UnitTest unit_test_t;

#define assert_raises(exception, block) 												\
	do {																				\
		bool_t thrown = TRUE, caught = FALSE;											\
		exception_t *e;																	\
		try { block; thrown = FALSE; } 													\
		catch(exception,e) { caught = TRUE;	} 											\
		except { }																		\
		_assert_true(caught && thrown, "exception '" #exception "' not raised", __FILE__, __LINE__); 	\
	} while(0);

