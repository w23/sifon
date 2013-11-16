#include "extern.h"
#include "cx_list.h"
#include "fscan_posix.h"

void fscan_posix(const char *path, metadata_read_f metaread, append_track_f append) {
  DIR *dir;
  struct dirent entry;
  struct dirent *pentry;

  cx_list_t *queue = cx_list_make();
  cx_list_insert(queue, cx_string_make(path), NULL);
  
  /* while there are directories left to scan */
  while(queue->front != NULL) {
    cx_list_item_t *nextdir = queue->front;
    cx_string_t *dirname = CX_LIST_ITEM(nextdir, cx_string_t);
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

      cx_string_t *fullname = cx_string_make(dirname->string);
      cx_string_append(fullname, "/");
      cx_string_append(fullname, entry.d_name);

      /* check entry type */
      struct stat st;
      if (0 != lstat(fullname->string, &st)) {
        perror(fullname->string);
        continue;
      }

      /* add directory to to-scan list */
      if(S_ISDIR(st.st_mode)) {
        cx_list_insert(queue, cx_string_make(fullname->string), NULL);
        /* scan regular file for metadata */
      } else if (S_ISREG(st.st_mode)) {
        tags_t *tags = metaread.func(metaread.param, fullname->string);
        if (tags != NULL) {
          track_info_t *track = track_info_make(fullname->string, tags);
          append.func(append.param, track);
          cx_release(track);
          cx_release(tags);
        }
      } /* if regular file */
      cx_release(fullname);
    } /* for all dir entries */
    closedir(dir);
    cx_list_remove(nextdir);
  } /* for all dirs to scan */

  cx_release(queue);
}
