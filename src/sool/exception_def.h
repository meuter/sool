#ifndef __EXCEPTION_DEF_H__
#define __EXCEPTION_DEF_H__

#include <sool/exception.h>
#include "object_def.h"

struct _exception_t {
	EXTENDS(object_t);
	const char *msg;
};

#define DEFINE_EXCEPTION(name) 																\
	class_t *name() {																		\
		static class_t *result = NULL;														\
		if (result == NULL)																	\
			result = new(Class(), __FUNCTION__, Exception(), sizeof(exception_t), NULL);	\
		return result;																		\
	}

#endif
