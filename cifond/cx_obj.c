#include "cx_obj.h"

void *cx_retain(void *o) {
  ++((cx_obj_t*)o)->refcount;
  return o;
}

void cx_release(void *o) {
  cx_obj_t *this = (cx_obj_t*)o;
  if (--(this->refcount) == 0) {
    this->dtor(this);
    free(this);
  }
}
