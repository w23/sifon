#include <sqlite3.h>

#include "cx_log.h"
#include "keeper.h"

static const char *keeper_class_name = "keeper";
static const char *sqlite_class_name = "keeper::sqlite";
static cx_string_ptr database_file;

CX_OBJ_BEGIN(sqlite)
  sqlite3 *db;
CX_OBJ_END(sqlite)

CX_OBJ_BEGIN(stmt)
  sqlite3_stmt *stmt;
CX_OBJ_END(stmt)

static void sqlite_dtor(void *sqlite) {
  sqlite_ptr this = (sqlite_ptr)sqlite;
  sqlite3_close(this->db);
}

static int keeper_exec(keeper_ptr this, const char *sql) {
  int err = sqlite3_exec(((sqlite_ptr)(this->impl))->db,
    sql, NULL, NULL, NULL);
  if (err != SQLITE_OK) 
    CX_LOG_ERROR("Error executins SQL {%s}: %s", sql,
      sqlite3_errmsg(((sqlite_ptr)(this->impl))->db));
  return err;
}

static sqlite_ptr impl_create() {
  sqlite_ptr this;
  sqlite3 *db;
  int err;
  
  err = sqlite3_open_v2(database_file->string, &db,
    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL);
  if (err != SQLITE_OK) {
    CX_LOG_ERROR("Error opening sqlite database \"%s\": %d",
      database_file->string, err);
    sqlite3_close(db);
    return NULL;
  }
  
  this = CX_CREATE(sqlite);
  this->db = db;
  return this;
}

static void keeper_dtor(void *keeper){
  keeper_ptr this = (keeper_ptr)keeper;
  cx_release(this->impl);
}

keeper_ptr keeper_create(const char *filename) {
  keeper_ptr this;

  CX_LOG("Creating keeper: %s", filename);
  database_file = cx_string_create(filename);

  this = keeper_copy(NULL);
  if (this == NULL) return NULL;

  // create tables
  if ((SQLITE_OK != keeper_exec(this,
      "CREATE TABLE tracks ("
        "id INTEGER PRIMARY KEY,"
        "filename TEXT UNIQUE"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE TABLE tag_rels ("
        "track_id INTEGER, tag_id INTEGER, UNIQUE(track_id, tag_id)"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE VIRTUAL TABLE tags USING fts4(key, value"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE TABLE instances ("
        "track_id INTEGER,"
        "preset TEXT,"
        "status INTEGER,"
        "filename TEXT UNIQUE,"
        "UNIQUE(track_id, preset)"
      ")"))) {
    CX_LOG_ERROR("Error creating tables");
    cx_release(this);
    return NULL;
  }
  return this;
}

keeper_ptr keeper_copy(keeper_ptr keeper) {
  CX_UNUSED(keeper);
  sqlite_ptr impl = impl_create();
  if (impl == NULL) return NULL;

  keeper_ptr this = CX_CREATE(keeper);
  this->impl = (cx_obj_ptr)impl;
  return this;
}

cx_iterator_ptr keeper_track_find(keeper_ptr keeper, cx_string_ptr needle) {
  CX_UNUSED(keeper);
  CX_LOG_INFO("Search for tracks with \"%s\"", needle->string);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}

void keeper_track_insert(keeper_ptr keeper, track_info_ptr track) {
  CX_UNUSED(keeper);
  CX_LOG_ERROR("Not implemented");
  CX_LOG_DEBUG("Append track: %s\n"
    "  codec: %s\n"
    "  bitrate: %d\n"
    "  duration: %d\n"
    "  samplerate: %d\n"
    "  channels: %d\n"
    "  tags(%zd):",
    track->filename->string, track->codec_name->string,
    track->bitrate,
    track->duration, track->sample_rate, track->channels,
    tags_count(track->tags));
  cx_size_t i;
  for (i = 0; i < tags_count(track->tags); ++i)
    CX_LOG_DEBUG("    %zd:%s = \"%s\"", i,
      tags_get(track->tags, i)->key->string,
      tags_get(track->tags, i)->value->string);


}

void keeper_instance_get(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
}

int keeper_instance_insert(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
  return 0;
}

void keeper_instance_update(keeper_ptr keeper, track_instance_ptr instance) {
  CX_UNUSED(keeper);
  CX_UNUSED(instance);
  CX_LOG_ERROR("Not implemented");
}


