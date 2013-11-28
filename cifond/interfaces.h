#ifndef __INTERFACES_H__
#define __INTERFACES_H__

struct track_info_read_f {
  void *param;
  track_info_ptr (*func)(void *param, const char *filename);
};

struct append_track_f {
  void *param;
  void (*func)(void *param, track_info_ptr track);
};

#endif /*ifndef __INTERFACES_H__*/

