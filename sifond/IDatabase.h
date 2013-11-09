#pragma once
#include "TrackInfo.h"

class IDatabase {
public:
  virtual ~IDatabase() {}
  
  //! reads tags
  //! fills timestamp and track_id
  //! throws
  virtual void submit(TrackInfo &track) = 0;
  
  //! reads filename, preset
  //! fills timestamp and view_id
  //! throws
  virtual void submit(TrackView &view) = 0;
  
  virtual TrackInfo_v find(const std::string &search_string) const = 0;
  virtual TrackInfo_v find(const Tags &search_tags) const = 0;
};