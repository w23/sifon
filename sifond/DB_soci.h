#pragma once
#include "stable.h"
#include "IDatabase.h"

class DB_soci : public IDatabase {
public:
  DB_soci(const std::string &dbname,
          const std::string &host = "localhost",
          const std::string &user = "",
          const std::string &password = "");
  
  virtual ~DB_soci() {}
  virtual void submit(const Tags &tags, TrackView &view);
  virtual TrackInfo_v find(const std::string &search_string) const;
  virtual TrackInfo_v find(const Tags &search_tags) const;
  
private:
  //! \todo thread-local databases are way more awesome
  std::mutex mutex_;
  
  soci::session sql_;
};

