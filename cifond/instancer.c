#include "cx_log.h"
#include "instancer.h"

cx_stream_ptr instance_track(track_info_ptr track, const char *preset) {
  if (preset == NULL)
    return cx_stream_create_file(track->filename->string);

  CX_LOG_INFO("Building instance %d/%s", track->id, preset);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}
