#pragma once
#include "TrackInfo.h"

class IDatabase {
public:
  virtual ~IDatabase() {}
  
  virtual void insert(TrackInfo &info, TrackView &origin) = 0;
  virtual void insert(TrackView &view) = 0;
  
  virtual TrackInfo_v find(const std::string &search_string) const = 0;
  virtual TrackInfo_v find(const Tags &search_tags) const = 0;
  
  virtual TrackView_v views(const TrackInfo &track) const = 0;
};