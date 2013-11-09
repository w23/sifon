#pragma once
#include "stable.h"
#include "IDatabase.h"

class DB_ram : public IDatabase {
public:
  DB_ram() {}
  
  virtual ~DB_ram() {}
  virtual void insert(TrackInfo &info, TrackView &origin);
  virtual void insert(TrackView &view);
  virtual TrackInfo_v find(const std::string &search_string) const;
  virtual TrackInfo_v find(const Tags &search_tags) const;
  virtual TrackView_v views(const TrackInfo &track) const;
  
private:
  struct TrackFullInfo {
    TrackInfo info;
    TrackView_v views;
    
    TrackFullInfo(const TrackInfo &in_info, const TrackView &origin)
    : info(in_info) { views.push_back(origin); }
  };
  
  std::map<std::int64_t, TrackFullInfo> tracks_;
};