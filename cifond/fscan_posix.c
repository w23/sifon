#include "extern.h"
#include "cx_list.h"
#include "fscan_posix.h"

void fscan_posix(const char *path, struct track_info_read_f trackread,
  struct append_track_f append) {
  DIR *dir;
  struct dirent entry;
  struct dirent *pentry;

  cx_list_ptr queue = cx_list_create();
  cx_string_ptr pathstr = cx_string_create(path);
  cx_list_insert(queue, pathstr, NULL);
  cx_release(pathstr);

  /* while there are directories left to scan */
  while(queue->front != NULL) {
    cx_list_item_ptr nextdir = queue->front;
    cx_string_ptr dirname = CX_LIST_ITEM(nextdir, cx_string);
    dir = opendir(dirname->string);
    if (dir == NULL) {
      perror(dirname->string);
      cx_list_remove(nextdir);
      continue;
    }

    /* go through directory contents */
    for (;;) {
      int err = readdir_r(dir, &entry, &pentry);
      if (err != 0) {
        // error err
        break;
      }

      /* stop scanning if there are no dir entries left */
      if (pentry == NULL) break;

      /* ignore dots */
      if ((0 == strcmp(entry.d_name, "."))
        || (0 == strcmp(entry.d_name, "..")))
        continue;

      cx_string_ptr fullname = cx_string_create(dirname->string);
      cx_string_append(fullname, "/");
      cx_string_append(fullname, entry.d_name);

      /* check entry type */
      struct stat st;
      if (0 != lstat(fullname->string, &st)) {
        perror(fullname->string);
        cx_release(fullname);
        continue;
      }

      /* add directory to to-scan list */
      if(S_ISDIR(st.st_mode)) {
        cx_list_insert(queue, fullname, NULL);
        /* scan regular file for metadata */
      } else if (S_ISREG(st.st_mode)) {
        track_info_ptr track = trackread.func(trackread.param, fullname->string);
        if (track != NULL) {
          append.func(append.param, track);
          cx_release(track);
        }
      } /* if regular file */
      cx_release(fullname);
    } /* for all dir entries */
    closedir(dir);
    cx_list_remove(nextdir);
  } /* for all dirs to scan */
  cx_release(queue);
}
