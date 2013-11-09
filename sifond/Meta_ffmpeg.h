#pragma once
#include "IMetadataReader.h"

class Meta_ffmpeg : public IMetadataReader {
public:
  virtual Tags read_tags(const std::string &filename) const;
};