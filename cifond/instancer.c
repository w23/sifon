#include "extern.h"
#include "cx_log.h"
#include "instancer.h"

static const char *decoder_stream_class_name = "decoder_stream";

struct decoder_stream_t {
  struct cx_stream_t header;
  int channels, samplerate;
  AVFormatContext *format_ctx;
  AVCodecContext *codec_ctx;
  AVStream *stream;
  AVCodec *codec;
  AVFrame *frame;
  AVPacket packet;
CX_OBJ_END(decoder_stream)

static void decoder_stream_dtor(void *stream) {
  decoder_stream_ptr this = (decoder_stream_ptr)stream;
  /// \todo free packet condition
  if (this->frame != NULL) avcodec_free_frame(&this->frame);
  if (this->codec_ctx != NULL) avcodec_close(this->codec_ctx);
  if (this->format_ctx != NULL) avformat_close_input(&this->format_ctx);
}

static struct cx_packet_t decoder_stream_read(cx_stream_ptr stream) {
  decoder_stream_ptr this = (decoder_stream_ptr)stream;
  struct cx_packet_t pkt;
  CX_LOG_ERROR("stream decoding is not implemented lols");
  CX_UNUSED(this);
  pkt.data = NULL;
  pkt.size = 0;
  return pkt;
}

static decoder_stream_ptr decoder_stream_create(const char *filename) {
  int err = 0;
  decoder_stream_ptr this = CX_CREATE(decoder_stream);
  this->header.read = decoder_stream_read;
  this->format_ctx = NULL;
  this->codec_ctx = NULL;
  this->codec = NULL;
  this->frame = NULL;
  this->packet.data = NULL;
  this->packet.size = 0;

  err = avformat_open_input(&this->format_ctx, filename, NULL, NULL);
  if (err < 0) {
    CX_LOG_ERROR("Couldn't open file \"%s\": %d", filename, err);
    cx_release(this);
    return NULL;
  }

  err = avformat_find_stream_info(this->format_ctx, NULL);
  if (err < 0) {
    CX_LOG_ERROR("Couldn't find streams in \"%s\": %d", filename, err);
    cx_release(this);
    return NULL;
  }

  int stream_index = av_find_best_stream(this->format_ctx,
    AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  if (stream_index < 0) {
    CX_LOG_ERROR("Couldn't find audio stream in \"%s\": %d",
      filename, stream_index);
    cx_release(this);
    return NULL;
  }

  this->stream = this->format_ctx->streams[stream_index];
  this->codec = avcodec_find_decoder(this->stream->codec->codec_id);
  if (this->codec == NULL) {
    CX_LOG_ERROR("Couldn't find audio decoder for file \"%d\"", filename);
    cx_release(this);
    return NULL;
  }

  err = avcodec_open2(this->stream->codec, this->codec, NULL);
  if (err < 0) {
    CX_LOG_ERROR("Couldn't open audio codec for file \"%s\": %d",
      filename, err);
    cx_release(this);
    return NULL;
  }
  this->codec_ctx = this->stream->codec;

  this->frame = avcodec_alloc_frame();
  av_init_packet(&this->packet);
  return this;
}

static cx_stream_ptr encoder_stream_factory_create(
  decoder_stream_ptr decoder, const char *preset) {
  CX_UNUSED(decoder);
  CX_UNUSED(preset);
  CX_LOG_ERROR("Encoders are not implemented");
  return NULL;
}

cx_stream_ptr instance_track(track_info_ptr track, const char *preset) {
  if (preset == NULL)
    return cx_stream_create_file(track->filename->string);

  CX_LOG_INFO("Building instance %d/%s", track->id, preset);

  decoder_stream_ptr decoder = decoder_stream_create(track->filename->string);
  if (decoder == NULL) {
    CX_LOG_ERROR("Cannot open decoder stream for track %d", track->id);
    return NULL;
  }

  cx_stream_ptr encoder = encoder_stream_factory_create(decoder, preset);
  cx_release(decoder);
  if (encoder == NULL) {
    CX_LOG_ERROR("Cannot create encoder \"%s\" for track %d", preset, track->id);
    return NULL;
  }

  return encoder;
}
