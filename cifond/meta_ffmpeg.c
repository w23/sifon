#include "extern.h"
#include "meta_ffmpeg.h"

static tags_t *meta_ffmpeg_func(void *param, const char *filename) {
  AVFormatContext *fmt_ctx = NULL;
  int err = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
  if (err != 0) return NULL;

  tags_t *tags = tags_make();
  AVDictionaryEntry *tag = NULL;
  while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    cx_array_insert_back(&tags->tags, tag_make(tag->key, tag->value));
  avformat_close_input(&fmt_ctx);
  return tags;
}

const metadata_read_f meta_ffmpeg = {
	NULL,
	meta_ffmpeg_func
};

