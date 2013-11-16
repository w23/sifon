#ifndef __CX_STRING_H__
#define __CX_STRING_H__
#include "cx_obj.h"

extern const char *cx_string_class_name;

struct cx_string_t { CX_O;
  cx_size_t length;
  char *string;
};
typedef struct cx_string_t *cx_string_ptr;

cx_string_ptr cx_string_create(const char *str);
cx_string_ptr cx_string_append(cx_string_ptr string, const char *suffix);

#endif /*ifndef __CX_STRING_H__*/
