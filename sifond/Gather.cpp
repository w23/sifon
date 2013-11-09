#include "Gather.h"

Gather::Gather(IDatabase *db, IMetadataReader *meta,
               const std::string &path) : db_(db), meta_(meta), path_(path) {
  run();
}

Gather::~Gather() {}

void Gather::run() {
  for (auto it = fs::recursive_directory_iterator(path_);
       it != fs::recursive_directory_iterator(); ++it) {
    if (fs::is_regular_file(it->path()))
      try {
        Tags tags(meta_->read_tags(it->path().string()));
        std::cerr << it->path() << ":" << std::endl << tags << std::endl;
      } catch (std::exception &e) {
        std::cerr << "error reading tags: " << e.what() << std::endl;
      }
  }
}