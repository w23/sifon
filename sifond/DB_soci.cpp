#include "DB_soci.h"

DB_soci::DB_soci(const std::string &dbname,
                 const std::string &host,
                 const std::string &user,
                 const std::string &password)
: sql_(soci::postgresql, std::string("host=") + host
              + " user=" + user
              + " password=" + password
              + " dbname=" + dbname) {
  sql_ << "CREATE SEQUENCE track_ids";
  
  sql_ << "CREATE TABLE IF NOT EXISTS tags ("
    "track_id BIGINT NOT NULL,"
    "key TEXT,"
    "value TEXT"
  ")";

  sql_ << "CREATE TABLE IF NOT EXISTS tracks ("
    "track_id BIGINT NOT NULL,"
    "timestamp TIMESTAMP,"
    "preset TEXT NOT NULL,"
    "filename TEXT NOT NULL"
  ")";
}

void DB_soci::submit(const Tags &tags, TrackView &view) {
  if (view.track_id == -1) {
    //! \todo check whether we have this file already
    std::int64_t tid;
    soci::indicator ind;
    sql_ << "SELECT track_id FROM tracks WHERE filename = :filename",
      soci::use(view.filename), soci::into(tid, ind);
    
    if (sql_.got_data()) {
      std::cerr << "already have file " << view.filename;
      return;
    }
    
    sql_ << "SELECT NEXTVAL(track_ids)", soci::into(tid);
    sql_ << "INSERT INTO tracks (track_id, preset, filename)"
      " VALUES(:track_id, :preset, :filename)",
      soci::use(tid), soci::use(view.preset), soci::use(view.filename);

    for (auto t: tags) {
      sql_ << "INSERT INTO tags VALUES(:tid, :key, :value)",
        soci::use(tid), soci::use(t.first), soci::use(t.second);
    }
  } else {
    sql_ << "INSERT INTO tracks (preset, filename) VALUES(:preset, :filename)",
      soci::use(view.preset), soci::use(view.filename);
  }
}

TrackInfo_v DB_soci::find(const std::string &search_string) const {
  return TrackInfo_v();
}

TrackInfo_v DB_soci::find(const Tags &search_tags) const {
  TrackInfo_v ret;
  return ret;
}
