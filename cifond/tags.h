#ifndef __TAGS_H__
#define __TAGS_H__
#include "cx_array.h"
#include "cx_string.h"

extern const char *tag_class_name;

CX_OBJ_BEGIN(tag)
  cx_string_ptr key, value;
CX_OBJ_END(tag)

tag_ptr tag_create();

typedef cx_array_ptr tags_ptr;

tags_ptr tags_create();
void tags_insert(tags_ptr tags, const char *key, const char *value);
void tags_insert_string(tags_ptr tags, cx_string_ptr key, cx_string_ptr value);
cx_size_t tags_count(tags_ptr);
tag_ptr tags_get(tags_ptr, cx_size_t index);

#endif /*ifndef __TAGS_H__*/
