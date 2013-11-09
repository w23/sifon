#pragma once
#include "stable.h"
#include "IDatabase.h"
#include "IMetadataReader.h"

namespace fs { using namespace boost::filesystem; }

class Gather {
public:
  Gather(IDatabase &db, IMetadataReader &meta, const std::string &path);
  ~Gather();
  
private:
  void run();

  IDatabase &db_;
  IMetadataReader &meta_;
  fs::path path_;
  //std::thread thread_;
};