#ifndef __CX_STRING_H__
#define __CX_STRING_H__
#include "cx_obj.h"

extern const char *cx_string_class_name;

typedef struct {
  CX_O;
  cx_size_t length;
  char *string;
} cx_string_t;

cx_string_t *cx_string_alloc();
cx_string_t *cx_string_init(cx_string_t *string, const char *str);
cx_string_t *cx_string_make(const char *str);
cx_string_t *cx_string_append(cx_string_t *string, const char *append);

#endif /*ifndef __CX_STRING_H__*/
