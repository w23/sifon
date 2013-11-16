#ifndef __CX_ARRAY_H__
#define __CX_ARRAY_H__
#include "cx_obj.h"

extern const char *cx_array_class_name;

typedef struct {
  CX_O;
  cx_obj_t **objs;
  cx_size_t count;
  cx_size_t reserved;
} cx_array_t;

cx_array_t *cx_array_make();
void cx_array_insert_back(cx_array_t *array, void *obj);

#endif /*ifndef __CX_ARRAY_H__*/
