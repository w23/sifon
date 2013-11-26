#include "extern.h"
#include "meta_ffmpeg.h"

static track_info_ptr track_info_ffmpeg_func(void *param, const char *filename) {
  (void)param;
  // Open file
  AVFormatContext *fmt_ctx = NULL;
  int err = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
  if (err != 0) return NULL;

  // Check that file does indeed contains audio
  err = avformat_find_stream_info(fmt_ctx, NULL);
  if (err < 0) {
    avformat_close_input(&fmt_ctx);
    return NULL;
  }

  // Read tags
  tags_ptr tags = tags_create();
  AVDictionaryEntry *tag = NULL;
  while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    tags_insert(tags, tag->key, tag->value);
  avformat_close_input(&fmt_ctx);

  track_info_ptr track = track_info_create(filename, tags);
  cx_release(tags);
  return track;
}

const struct track_info_read_f meta_ffmpeg = {
  NULL,
  track_info_ffmpeg_func
};

