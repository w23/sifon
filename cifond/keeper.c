#include "cx_log.h"
#include "keeper.h"

static const char *keeper_class_name = "keeper";

static void keeper_dtor(void *keeper){
  keeper_ptr this = (keeper_ptr)keeper;
  cx_release(this->impl);
}

keeper_ptr keeper_create(const char *filename) {
  CX_LOG("Creating keeper: %s", filename);
  keeper_ptr this = CX_CREATE(keeper);
  this->impl = NULL;
  return this;
}

keeper_ptr keeper_copy(keeper_ptr keeper) {
  CX_UNUSED(keeper);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}

cx_iterator_ptr keeper_track_find(keeper_ptr keeper, cx_string_ptr needle) {
  CX_UNUSED(keeper);
  CX_LOG_INFO("Search for tracks with \"%s\"", needle->string);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}

void keeper_track_insert(keeper_ptr keeper, track_info_ptr track) {
  CX_UNUSED(keeper);
  CX_LOG_ERROR("Not implemented");
  CX_LOG_DEBUG("Append track: %s\n"
    "  codec: %s\n"
    "  bitrate: %d\n"
    "  duration: %d\n"
    "  samplerate: %d\n"
    "  channels: %d\n"
    "  tags(%zd):",
    track->filename->string, track->codec_name->string,
    track->bitrate,
    track->duration, track->sample_rate, track->channels,
    tags_count(track->tags));
  cx_size_t i;
  for (i = 0; i < tags_count(track->tags); ++i)
    CX_LOG_DEBUG("    %zd:%s = \"%s\"", i,
      tags_get(track->tags, i)->key->string,
      tags_get(track->tags, i)->value->string);
}

void keeper_instance_get(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
}

int keeper_instance_insert(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
  return 0;
}

void keeper_instance_update(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
}


