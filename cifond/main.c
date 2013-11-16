#include "extern.h"
#include "fscan_posix.h"
#include "meta_ffmpeg.h"

static tags_t *dummy_metadata_reader(void *param, const char *filename) {
  tags_t *tags = tags_make();
  cx_array_insert_back(&tags->tags, tag_make("filename", filename));
  cx_array_insert_back(&tags->tags, tag_make("dummy", "dummy"));
  return tags;
}

static void dummy_append_track(void *param, track_info_t *track) {
  fprintf(stdout, "Append track: %s\n  Tags(%zd):\n",
    track->filename->string, tags_count(track->tags));
  cx_size_t i;
  for (i = 0; i < tags_count(track->tags); ++i)
    fprintf(stdout, "    %zd:%s = \"%s\"\n", i,
      tags_get(track->tags, i)->key->string,
      tags_get(track->tags, i)->value->string);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path_to_scan>\n", argv[0]);
    return 1;
  }

  av_register_all();

  metadata_read_f metaread;
  metaread.param = NULL;
  metaread.func = dummy_metadata_reader;

  append_track_f append;
  append.param = NULL;
  append.func = dummy_append_track;

  fscan_posix(argv[1], meta_ffmpeg, append);
  //fscan_posix(argv[1], metaread, append);
  return 0;
}
