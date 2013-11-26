#ifndef __TRACK_INFO_H__
#define __TRACK_INFO_H__
#include "tags.h"

extern const char *track_info_clas_name;

struct track_info_t { CX_O;
  cx_string_ptr filename;
  tags_ptr tags;
};
typedef struct track_info_t *track_info_ptr;

track_info_ptr track_info_create(const char *filename, tags_ptr tags);

struct track_info_read_f {
  void *param;
  track_info_ptr (*func)(void *param, const char *filename);
};

struct append_track_f {
  void *param;
  void (*func)(void *param, track_info_ptr track);
};

#endif /*ifndef __TRACK_INFO_H__*/
