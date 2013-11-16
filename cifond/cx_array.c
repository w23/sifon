#include "cx_array.h"

const char *cx_array_class_name = "cx_array";

static void cx_array_dtor(void *array) {
  cx_array_ptr this = (cx_array_ptr)array;
  cx_size_t i;
  for(i = 0; i < this->count; ++i)
    cx_release(this->objs[i]);
  free(this->objs);
}

cx_array_ptr cx_array_create() {
  cx_array_ptr this = CX_CREATE(cx_array);
  this->count = 0;
  this->reserved = 4;
  this->objs = (cx_obj_ptr*)malloc(this->reserved * sizeof(cx_obj_ptr));
  return this;
}

void cx_array_insert_back(cx_array_ptr this, void *obj) {
  if (this->count == this->reserved) {
    this->reserved *= 2;
    this->objs = (cx_obj_ptr*)realloc(this->objs, this->reserved * sizeof(cx_obj_ptr));
  }
  this->objs[this->count] = cx_retain(obj);
  ++this->count;
}
