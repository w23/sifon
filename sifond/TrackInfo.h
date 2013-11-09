#pragma once
#include "stable.h"

typedef std::unordered_map<std::string, std::string> Tags;

struct TrackInfo {
  std::uint64_t timestamp;
  std::uint64_t track_id;
  Tags tags;
};
typedef std::vector<TrackInfo> TrackInfo_v;

struct TrackView {
  std::uint64_t timestamp;
  std::uint64_t track_id;
  std::uint64_t view_id;
  std::string filename;
  std::string preset;
};
typedef std::vector<TrackView> TrackView_v;

inline std::ostream &operator<<(std::ostream &s, const Tags &tags) {
  for (auto i: tags) {
    s << "\t" << i.first << ": " << i.second << std::endl;
  }
  return s;
}