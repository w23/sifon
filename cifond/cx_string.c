#include <memory.h>
#include <string.h>
#include "cx_string.h"

const char *cx_string_class_name = "cx_string";

static void cx_string_dtor(void *s) {
  free(((cx_string_ptr)s)->string);
}

cx_string_ptr cx_string_create(const char *source) {
  cx_string_ptr this = CX_CREATE(cx_string);
  this->length = strlen(source);
  this->string = malloc(this->length + 1);
  memcpy(this->string, source, this->length + 1);
  return this;
}

cx_string_ptr cx_string_append(cx_string_ptr this, const char *suffix) {
  cx_size_t suffix_length = strlen(suffix);
  this->string = realloc(this->string, this->length + suffix_length + 1);
  memcpy(this->string + this->length, suffix, suffix_length + 1);
  this->length += suffix_length;
  return this;
}
