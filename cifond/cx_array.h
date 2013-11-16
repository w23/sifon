#ifndef __CX_ARRAY_H__
#define __CX_ARRAY_H__
#include "cx_obj.h"

extern const char *cx_array_class_name;

struct cx_array_t { CX_O;
  cx_obj_ptr *objs;
  cx_size_t count;
  cx_size_t reserved;
};
typedef struct cx_array_t *cx_array_ptr;

cx_array_ptr cx_array_create();
void cx_array_insert_back(cx_array_ptr array, void *obj);

#define CX_ARRAY_AT(A,I,T) ((T##_ptr)(((cx_array_ptr)A)->objs[I]))

#endif /*ifndef __CX_ARRAY_H__*/
