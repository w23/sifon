#ifndef __FSCAN_POSIX_H__
#define __FSCAN_POSIX_H__
#include "track_info.h"

void fscan_posix(const char *path,
  struct metadata_read_f metaread, struct append_track_f append);

#endif /* ifndef __FSCAN_POSIX_H__ */
