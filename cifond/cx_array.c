#include "cx_array.h"

const char *cx_array_class_name = "cx_array";

static void cx_array_dtor(void *array) {
  cx_array_t *this = (cx_array_t*)array;
  cx_size_t i;
  for(i = 0; i < this->count; ++i)
    cx_release(this->objs[i]);
}

cx_array_t *cx_array_make() {
  cx_array_t *this = (cx_array_t*)malloc(sizeof(cx_array_t));
  this->O.class_name = cx_array_class_name;
  this->O.refcount = 1;
  this->O.dtor = cx_array_dtor;
  this->count = 0;
  this->reserved = 4;
  this->objs = (cx_obj_t**)malloc(this->reserved * sizeof(cx_obj_t*));
  return this;
}

void cx_array_insert_back(cx_array_t *this, void *obj) {
  if (this->count == this->reserved) {
    this->reserved *= 2;
    this->objs = (cx_obj_t**)realloc(this->objs, this->reserved * sizeof(cx_obj_t*));
  }
  this->objs[this->count] = cx_retain(obj);
  ++this->count;
}
