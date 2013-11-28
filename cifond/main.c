#include "extern.h"
#include "fscan_posix.h"
#include "meta_ffmpeg.h"
#include "cx_log.h"

static void dummy_append_track(void *param, track_info_ptr track) {
  (void)param;
  fprintf(stdout, "Append track: %s\n"
    "  codec: %s\n"
    "  bitrate: %d\n"
    "  duration: %d\n"
    "  samplerate: %d\n"
    "  channels: %d\n"
    "  Tags(%zd):\n",
    track->filename->string, track->codec_name->string,
    track->bitrate,
    track->duration, track->sample_rate, track->channels,
    tags_count(track->tags));
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

  cx_log_level = CX_LOG_LEVEL_DEBUG;

  CX_LOG("################");
  CX_LOG("Staring cifond with media source dir %s", argv[1]);

  av_register_all();

  struct append_track_f append;
  append.param = NULL;
  append.func = dummy_append_track;

  fscan_posix(argv[1], meta_ffmpeg, append);
  return 0;
}
