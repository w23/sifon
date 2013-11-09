#include "Gather.h"

Gather::Gather(IDatabase &db, IMetadataReader &meta,
               const std::string &path) : db_(db), meta_(meta), path_(path) {
  run();
}

Gather::~Gather() {}

void Gather::run() {
  for (auto it = fs::recursive_directory_iterator(path_);
       it != fs::recursive_directory_iterator(); ++it) {
    if (fs::is_regular_file(it->path()))
      try {
        std::string filename(it->path().string());
        TrackInfo track(meta_.read_tags(filename));
        TrackView origin(filename, "origin");
        db_.insert(track, origin);
        //std::cerr << it->path() << ":" << std::endl << tags << std::endl;
      } catch (std::exception &e) {
        std::cerr << "error reading tags: " << e.what() << std::endl;
      }
  }
}