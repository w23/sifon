#pragma once
#include "stable.h"

typedef std::unordered_map<std::string, std::string> Tags;

struct TrackInfo {
  std::uint64_t timestamp;
  std::int64_t track_id;
  Tags tags;
  
  TrackInfo(std::int64_t track) : timestamp(0), track_id(track) {}
  TrackInfo(Tags &&in_tags) : timestamp(0), track_id(-1), tags(in_tags) {}
};
typedef std::vector<TrackInfo> TrackInfo_v;

struct TrackView {
  std::uint64_t timestamp;
  std::int64_t track_id;
  std::string filename;
  std::string preset;
  
  TrackView(const std::string &in_filename, const std::string &in_preset)
  : timestamp(0), track_id(-1), filename(in_filename), preset(in_preset) {}
};
typedef std::vector<TrackView> TrackView_v;

inline std::ostream &operator<<(std::ostream &s, const Tags &tags) {
  for (auto i: tags) {
    s << "\t" << i.first << ": " << i.second << std::endl;
  }
  return s;
}