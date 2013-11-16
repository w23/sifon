#include "cx_obj.h"

void *cx_create(cx_size_t size, const char *classname, void (*dtor)(void*)) {
  cx_obj_ptr this = (cx_obj_ptr)malloc(size);
  this->class_name = classname;
  this->refcount = 1;
  this->dtor = dtor;
  return this;
}

void *cx_retain(void *o) {
  ++((cx_obj_ptr)o)->refcount;
  return o;
}

void cx_release(void *o) {
  cx_obj_ptr this = (cx_obj_ptr)o;
  if (--(this->refcount) == 0) {
    this->dtor(this);
    free(this);
  }
}
