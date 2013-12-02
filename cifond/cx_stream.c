#include <stdio.h>
#include "cx_log.h"
#include "cx_stream.h"

static const char *cx_stream_file_class_name = "cx_stream";

struct cx_stream_file_t {
  struct cx_stream_t stream;
  FILE *file;
  char buffer[4096];
CX_OBJ_END(cx_stream_file)

static void cx_stream_file_dtor(void *stream) {
  cx_stream_file_ptr this = (cx_stream_file_ptr)stream;
  fclose(this->file);
}

static struct cx_packet_t cx_stream_file_read(cx_stream_ptr stream) {
  cx_stream_file_ptr this = (cx_stream_file_ptr)stream;
  struct cx_packet_t pkt;
  pkt.data = this->buffer;
  pkt.size = fread(this->buffer, 1, sizeof(this->buffer), this->file);
  return pkt;
}

cx_stream_ptr cx_stream_create_file(const char *filename) {
  CX_LOG_DEBUG("Creating stream for file %s", filename);
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    CX_LOG_WARNING("Couldn't open file %s", filename);
    return NULL;
  }
  cx_stream_file_ptr this = CX_CREATE(cx_stream_file);
  this->stream.read = cx_stream_file_read;
  this->file = f;
  return (cx_stream_ptr)this;
}
