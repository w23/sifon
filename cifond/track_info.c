#include "track_info.h"

const char *track_info_class_name = "track_info";

static void track_info_dtor(void *track) {
  cx_release(((track_info_ptr)track)->filename);
  cx_release(((track_info_ptr)track)->tags);
  cx_release(((track_info_ptr)track)->codec_name);
}

track_info_ptr track_info_create(const char *filename, tags_ptr tags) {
  track_info_ptr this = CX_CREATE(track_info);
  this->filename = cx_string_create(filename);
  this->tags = cx_retain(tags);
  return this;
}
