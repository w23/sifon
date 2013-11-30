#ifndef __INSTANCER_H__
#define __INSTANCER_H__

#include "track_info.h"
#include "keeper.h"

void instancer_start();
void instancer_stop();

track_instance_ptr instance_track(keeper_ptr keeper,
  track_id_t id, const char *preset);

#endif /*ifndef __INSTANCER_H__*/

