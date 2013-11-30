#include "tags.h"

const char *tag_class_name = "tag";

void tag_dtor(void *obj) {
  tag_ptr this = (tag_ptr)obj;
  cx_release(this->key);
  cx_release(this->value);
}

tag_ptr tag_create_string(cx_string_ptr key, cx_string_ptr value) {
  tag_ptr this = CX_CREATE(tag);
  this->key = cx_retain(key);
  this->value = cx_retain(value);
  return this;
}

tag_ptr tag_create(const char *key, const char *value) {
  tag_ptr this = CX_CREATE(tag);
  this->key = cx_string_create(key);
  this->value = cx_string_create(value);
  return this;
}

tags_ptr tags_create() {
  return cx_array_create();
}

void tags_insert(tags_ptr this, const char *key, const char *value) {
  tag_ptr tag = tag_create(key, value);
  cx_array_insert_back(this, tag);
  cx_release(tag);
}

void tags_insert_string(tags_ptr this, cx_string_ptr key, cx_string_ptr value) {
  tag_ptr tag = tag_create_string(key, value);
  cx_array_insert_back(this, tag);
  cx_release(tag);
}

cx_size_t tags_count(tags_ptr this) {
  return this->count;
}

tag_ptr tags_get(tags_ptr this, cx_size_t index) {
  return CX_ARRAY_AT(this, index, tag);
}
