#ifndef __TRACK_INFO_H__
#define __TRACK_INFO_H__
#include "tags.h"

extern const char *track_info_clas_name;

struct track_info_t { CX_O;
  cx_string_ptr filename;
  cx_u32_t flags;
  int duration;
  int sample_rate;
  int channels;
  int bitrate;
  cx_string_ptr codec_name;
  tags_ptr tags;
};
typedef struct track_info_t *track_info_ptr;

#define TRACK_INFO_FLAG_LOSSLESS 1

track_info_ptr track_info_create(const char *filename, tags_ptr tags);

#endif /*ifndef __TRACK_INFO_H__*/
