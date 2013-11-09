#pragma once
#include "TrackInfo.h"

class IMetadataReader {
public:
  virtual ~IMetadataReader() {}
  virtual Tags read_tags(const std::string &filename) const = 0;
};