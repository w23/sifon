#ifndef __INSTANCER_H__
#define __INSTANCER_H__

#include "track_info.h"
#include "cx_stream.h"

cx_stream_ptr instance_track(track_info_ptr track, const char *preset);

#endif /*ifndef __INSTANCER_H__*/

