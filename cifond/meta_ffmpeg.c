#include "extern.h"
#include "meta_ffmpeg.h"

static tags_ptr meta_ffmpeg_func(void *param, const char *filename) {
  AVFormatContext *fmt_ctx = NULL;
  int err = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
  if (err != 0) return NULL;

  tags_ptr tags = tags_create();
  AVDictionaryEntry *tag = NULL;
  while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    tags_insert(tags, tag->key, tag->value);
  avformat_close_input(&fmt_ctx);
  return tags;
}

const struct metadata_read_f meta_ffmpeg = {
  NULL,
  meta_ffmpeg_func
};

