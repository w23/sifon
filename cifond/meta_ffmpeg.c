#include "extern.h"
#include "cx_log.h"
#include "meta_ffmpeg.h"

static track_info_ptr read_track_info(AVFormatContext *fmt_ctx,
  const char *filename) {
  int duration;
  cx_u32_t flags = 0;
  int sample_rate;
  int channels;
  int bitrate;
  int audio_streams = 0;
  cx_string_ptr codec_name = NULL;

  int err = avformat_find_stream_info(fmt_ctx, NULL);
  if (err < 0) {
    CX_LOG_INFO("(%s) cannot find any media streams", filename);
    return NULL;
  }

  for (unsigned i = 0; i < fmt_ctx->nb_streams; ++i) {
    AVStream *st = fmt_ctx->streams[i];
    if (st->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
      ++audio_streams;
      duration = st->duration * st->time_base.num / st->time_base.den;
      sample_rate = st->codec->sample_rate;
      channels = st->codec->channels;
      codec_name = cx_string_create(avcodec_get_name(st->codec->codec_id));
      bitrate = st->codec->bit_rate / 1000;
      /// \todo lossless?
    } else if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      CX_LOG_INFO("(%s) found video stream, skipping file", filename);
      cx_release(codec_name);
      return NULL;
    }
  }

  if (audio_streams != 1) {
    CX_LOG_INFO("(%s) no audio streams found", filename);
    cx_release(codec_name);
    return NULL;
  }

  // Read tags
  tags_ptr tags = tags_create();
  AVDictionaryEntry *tag = NULL;
  while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    tags_insert(tags, tag->key, tag->value);

  track_info_ptr track = track_info_create(filename, tags);
  track->flags = flags;
  track->duration = duration;
  track->sample_rate = sample_rate;
  track->channels = channels;
  track->bitrate = bitrate;
  track->codec_name = codec_name;
  cx_release(tags);
  return track;
}

static track_info_ptr track_info_ffmpeg_func(void *p, const char *filename) {
  (void)p;
  CX_LOG_DEBUG("(%s) probing file", filename);

  AVFormatContext *fmt_ctx = NULL;
  int err = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
  if (err != 0) {
    CX_LOG_INFO("(%s) cannot open file", filename);
    return NULL;
  }

  track_info_ptr track = read_track_info(fmt_ctx, filename);
  avformat_close_input(&fmt_ctx);
  return track;
}

const struct track_info_read_f meta_ffmpeg = {
  NULL,
  track_info_ffmpeg_func
};

