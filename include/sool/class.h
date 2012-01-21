#ifndef __CLASS_H__
#define __CLASS_H__

#include <stdlib.h>
#include <stdio.h>

#include <sool/args.h>
#include <sool/bool.h>

typedef void (*ctor_t) (void *self, va_list *args);
typedef void (*dtor_t) (void *self);
typedef int  (*put_t)  (void *self, FILE *stream, const char *format);
typedef int  (*cmp_t)  (void *self, void *other);

struct _class_t;
typedef struct _class_t class_t;

#define delete(args...) _delete(__VA_N_ARGS__(args), ##args)

class_t *Class(); /* new(Class(), "DerivedClass", SuperClass(), sizeof(derived_t),
				 		 ctor, derived_ctor,
			 			 dtor, derived_dtor,
		 				 put,  derived_put,
						 cmp,  derived_cmp
					 );
                   */

void  *new     (const class_t *class, ...);
void  _delete  (int n, ...);

void   ctor(void *self, va_list *args);
void   dtor(void *self);
int    put (void *self, FILE *stream, const char *format);
int    cmp (void *self, void *other);

int    putof   (void *self, const char *format);
int    fputof  (void *self, FILE *stream, const char *format);
int    puto    (void *self);
int    fputo   (void *self, FILE *stream);

bool_t equal   (void *self, void *other);



const class_t *class_of(void *self);
size_t         size_of(void *self);


#endif
