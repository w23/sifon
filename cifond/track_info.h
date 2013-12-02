#ifndef __TRACK_INFO_H__
#define __TRACK_INFO_H__
#include "tags.h"

extern const char *track_info_class_name;

typedef cx_u32_t track_id_t;

CX_OBJ_BEGIN(track_info)
  track_id_t id;
  cx_string_ptr filename;
  cx_u32_t flags;
  int duration;
  int sample_rate;
  int channels;
  int bitrate;
  cx_string_ptr codec_name;
  tags_ptr tags;
CX_OBJ_END(track_info)

#define TRACK_INFO_FLAG_LOSSLESS 1

track_info_ptr track_info_create(const char *filename, tags_ptr tags);

#endif /*ifndef __TRACK_INFO_H__*/
