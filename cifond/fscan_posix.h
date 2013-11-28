#ifndef __FSCAN_POSIX_H__
#define __FSCAN_POSIX_H__
#include "track_info.h"
#include "interfaces.h"

void fscan_posix(const char *path,
  struct track_info_read_f trackread, struct append_track_f append);

#endif /* ifndef __FSCAN_POSIX_H__ */
