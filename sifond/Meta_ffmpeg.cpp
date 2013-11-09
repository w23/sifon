#include "Meta_ffmpeg.h"

Tags Meta_ffmpeg::read_tags(const std::string &filename) const {
  AVFormatContext *fmt_ctx = NULL;
  int err = avformat_open_input(&fmt_ctx, filename.c_str(), NULL, NULL);
  if (err != 0)
    throw std::runtime_error("avformat cannot open file " + filename);
  
  AVDictionaryEntry *tag = NULL;
  Tags ret;
  while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    ret.emplace(tag->key, tag->value);
  avformat_close_input(&fmt_ctx);
  
  return ret;
}