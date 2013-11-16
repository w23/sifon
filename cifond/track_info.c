#include "track_info.h"

const char *tag_class_name = "tag";
const char *track_info_class_name = "track_info";

static void tag_dtor(void *tag) {
  cx_release(((tag_t*)tag)->key);
  cx_release(((tag_t*)tag)->value);
}

tag_t *tag_make(const char *key, const char *value) {
  tag_t *this = (tag_t*)malloc(sizeof(tag_t));
  this->O.class_name = tag_class_name;
  this->O.refcount = 1;
  this->O.dtor = tag_dtor;
  this->key = cx_string_make(key);
  this->value = cx_string_make(value);
  return this;
}

tags_t *tags_make() {
  return (tags_t*)cx_array_make();
}

cx_size_t tags_count(tags_t *tags) {
  return tags->tags.count;
}

const tag_t *tags_get(tags_t *tags, cx_size_t index) {
  return (tag_t*)(tags->tags.objs[index]);
}

static void track_info_dtor(void *track) {
  cx_release(((track_info_t*)track)->filename);
  cx_release(((track_info_t*)track)->tags);
}

track_info_t *track_info_make(const char *filename, tags_t *tags) {
  track_info_t *this = (track_info_t*)malloc(sizeof(track_info_t));
  this->O.class_name = track_info_class_name;
  this->O.refcount = 1;
  this->O.dtor = track_info_dtor;
  this->filename = cx_string_make(filename);
  this->tags = cx_retain(tags);
  return this;
}
