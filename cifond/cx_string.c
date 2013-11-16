#include <memory.h>
#include <string.h>
#include "cx_string.h"

const char *cx_string_class_name = "cx_string";

static void cx_string_dtor(void *s) {
  free(((cx_string_t*)s)->string);
}

cx_string_t *cx_string_alloc() {
  return (cx_string_t*)malloc(sizeof(cx_string_t));
}

cx_string_t *cx_string_init(cx_string_t *s, const char *source) {
  s->O.class_name = cx_string_class_name;
  s->O.refcount = 1;
  s->O.dtor = cx_string_dtor;

  s->length = strlen(source);
  s->string = malloc(s->length + 1);
  memcpy(s->string, source, s->length + 1);
  return s;
}

cx_string_t *cx_string_make(const char *str) {
  return cx_string_init(cx_string_alloc(), str);
}

cx_string_t *cx_string_append(cx_string_t *string, const char *append) {
  cx_size_t len = strlen(append);
  string->string = realloc(string->string, string->length + len + 1);
  memcpy(string->string + string->length, append, len + 1);
  string->length += len;
  return string;
}
