#ifndef __TRACK_INFO_H__
#define __TRACK_INFO_H__
#include "cx_array.h"
#include "cx_string.h"

extern const char *tag_class_name;

typedef struct { CX_O;
  cx_string_t *key;
  cx_string_t *value;
} tag_t;

tag_t *tag_make(const char *key, const char *value);

typedef struct {
  cx_array_t tags;
} tags_t;

tags_t *tags_make();
cx_size_t tags_count(tags_t *tags);
const tag_t *tags_get(tags_t *tags, cx_size_t index);

typedef struct {
  void *param;
  tags_t *(*func)(void *param, const char *filename);
} metadata_read_f;

extern const char *track_info_clas_name;

typedef struct { CX_O;
  cx_string_t *filename;
  tags_t *tags;
} track_info_t;

track_info_t *track_info_make(const char *filename, tags_t *tags);

typedef struct {
  void *param;
  void (*func)(void *param, track_info_t *track);
} append_track_f;

#endif /*ifndef __TRACK_INFO_H__*/
