#pragma once
#include "stable.h"

class IDatabase;

class IStream {
public:
  struct packet_t {
    const void *data;
    std::size_t size;
  };
  virtual ~IStream() {}
  virtual std::string mime_type() const = 0;
  virtual packet_t get_next_packet() = 0;
};

class IEncoder {
public:
  virtual ~IEncoder() {}
  virtual IStream *reencode_file(const std::string &filename) const;
};

class Scatter {
public:
  Scatter(IDatabase &db);
  
  void add_preset(const std::string &preset, IEncoder *encoder);
  IStream *get_track_view(std::int64_t track_id, const std::string &view) const;
  
private:
  IDatabase &db_;
  std::map<std::string, std::unique_ptr<IEncoder> > presets_;
};