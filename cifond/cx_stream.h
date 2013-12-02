#ifndef __CX_STREAM_H__
#define __CX_STREAM_H__

#include "cx_obj.h"

struct cx_packet_t {
  size_t size;
  const void *data;
};

CX_OBJ_BEGIN(cx_stream)
  struct cx_packet_t (*read)(struct cx_stream_t *stream);
CX_OBJ_END(cx_stream)

#define CX_STREAM_READ(s) ((s)->read(s))

cx_stream_ptr cx_stream_create_file(const char *filename);

#endif /*ifndef __CX_STREAM_H__*/
