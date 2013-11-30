#include "cx_log.h"
#include "instancer.h"

void instancer_start() {
}

void instancer_stop() {
}

track_instance_ptr instance_track(keeper_ptr keeper,
  track_id_t id, const char *preset) {
  CX_UNUSED(keeper);
  CX_LOG_INFO("Instance %d/%s", id, preset);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}
