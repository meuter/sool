#ifndef __EXCEPTION_DEF_H__
#define __EXCEPTION_DEF_H__

#include <sool/exception.h>
#include "object_def.h"

struct _exception_t {
	EXTENDS(object_t);
	const char *msg;
};

#endif
