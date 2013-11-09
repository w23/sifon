#include "DB_ram.h"

void DB_ram::insert(TrackInfo &info, TrackView &origin) {
  //! \todo why don't you check for duplicate tracks?
  //! \wontfix because its temporary you asshole!
  info.track_id = tracks_.empty() ? 0 : tracks_.crbegin()->first + 1;
  tracks_.emplace(info.track_id, TrackFullInfo(info, origin));
}

void DB_ram::insert(TrackView &view) {
  auto it = tracks_.find(view.track_id);
  if (it == tracks_.end())
    throw std::runtime_error("No track found with track_id="
                             + std::to_string(view.track_id));
  
  //! \todo should check for duplicates also
  it->second.views.push_back(view);
}

TrackInfo_v DB_ram::find(const std::string &search_string) const {
  TrackInfo_v ret;
  for (const auto t: tracks_) {
    // todo
  }
  return ret;
}

bool stringisubstring(const std::string &a, const std::string &b) {
  return std::search(a.begin(), a.end(), b.begin(), b.end()) != a.end();
}

TrackInfo_v DB_ram::find(const Tags &search_tags) const {
  TrackInfo_v ret;
  for (const auto t: tracks_) {
    bool good = true;
    for (auto tag: search_tags) {
      auto tt = t.second.info.tags.find(tag.first);
      if (tt == t.second.info.tags.end()
          || !stringisubstring(tt->second, tag.second)) {
        good = false;
        break;
      }
    }
    if (good)
      ret.push_back(t.second.info);
  }
  return ret;
}

TrackView_v DB_ram::views(const TrackInfo &track) const {
  auto it = tracks_.find(track.track_id);
  if (it == tracks_.end()) return TrackView_v();
  return it->second.views;
}
