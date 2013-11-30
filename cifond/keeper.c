#include <sqlite3.h>

#include "cx_log.h"
#include "keeper.h"

static const char *keeper_class_name = "keeper";
static const char *sqlite_class_name = "keeper::sqlite";
static const char *stmt_class_name = "keeper::stmt";

static cx_string_ptr database_file;

CX_OBJ_BEGIN(stmt)
  sqlite3 *db;
  sqlite3_stmt *stmt;
CX_OBJ_END(stmt)

CX_OBJ_BEGIN(sqlite)
  sqlite3 *db;
  stmt_ptr tag_rels_insert;
  stmt_ptr tags_insert;
  stmt_ptr track_insert;
  stmt_ptr track_find;
  stmt_ptr track_tags;
CX_OBJ_END(sqlite)

#define KEEPER_DB(keeper) (((sqlite_ptr)((keeper)->impl))->db)

static void sqlite_dtor(void *sqlite) {
  sqlite_ptr this = (sqlite_ptr)sqlite;
  cx_release(this->tag_rels_insert);
  cx_release(this->tags_insert);
  cx_release(this->track_insert);
  cx_release(this->track_find);
  cx_release(this->track_tags);
  sqlite3_close(this->db);
}

static void stmt_dtor(void *stmt) {
  stmt_ptr this = (stmt_ptr)stmt;
  sqlite3_finalize(this->stmt);
}

static void keeper_dtor(void *keeper){
  keeper_ptr this = (keeper_ptr)keeper;
  cx_release(this->impl);
}

static stmt_ptr stmt_create(sqlite_ptr impl, const char *sql) {
  sqlite3_stmt *stmt;
  int err;

  err = sqlite3_prepare_v2(impl->db, sql, -1, &stmt, NULL);
  if (err != SQLITE_OK) {
    CX_LOG_ERROR("Error creating prepared statement with SQL {%s}: %s", sql,
      sqlite3_errmsg(impl->db));
    return NULL;
  }

  stmt_ptr this = CX_CREATE(stmt);
  this->db = impl->db;
  this->stmt = stmt;
  return this;
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

static void stmt_reset(stmt_ptr stmt) { sqlite3_reset(stmt->stmt); }

static int stmt_bind_int(stmt_ptr this, int index, int value) {
  int err = sqlite3_bind_int(this->stmt, index + 1, value);
  if (err != SQLITE_OK)
    CX_LOG_ERROR("Error binding int to %d: %s", index, sqlite3_errstr(err));
  return err;
}

static int stmt_bind_string(stmt_ptr this, int index, const cx_string_ptr value) {
  int err = sqlite3_bind_text(this->stmt, index + 1,
    value->string, value->length, SQLITE_TRANSIENT);
  if (err != SQLITE_OK)
    CX_LOG_ERROR("Error binding text to %d: %s", index, sqlite3_errstr(err));
  return err;
}

static int stmt_exec(stmt_ptr this) {
  int result = sqlite3_step(this->stmt);
  switch (result) {
  case SQLITE_DONE:
    return SQLITE_OK;
  case SQLITE_ROW:
    CX_LOG_WARNING("Statement returns an unexpected result.");
    return SQLITE_OK;
  }
  CX_LOG_ERROR("Error executing statement: (%d) %s", result,
    sqlite3_errstr(result));
  return -1;
}

static int keeper_exec(keeper_ptr this, const char *sql) {
  int err = sqlite3_exec(KEEPER_DB(this),
    sql, NULL, NULL, NULL);
  if (err != SQLITE_OK) {
    CX_LOG_ERROR("Error executins SQL {%s}: %s", sql,
      sqlite3_errmsg(KEEPER_DB(this)));
  }
  return err;
}

static keeper_ptr keeper_raw_create() {
  sqlite_ptr impl = impl_create();
  if (impl == NULL) return NULL;

  keeper_ptr this = CX_CREATE(keeper);
  this->impl = (cx_obj_ptr)impl;
  return this;
}

static void impl_init_statements(cx_obj_ptr impl) {
  sqlite_ptr this = (sqlite_ptr)impl;

  this->tag_rels_insert = stmt_create(this,
    "INSERT OR IGNORE INTO tag_rels (track_id, tag_id) VALUES (?, ?)");

  this->tags_insert = stmt_create(this,
    "INSERT OR ROLLBACK INTO tags (key, value) VALUES (?, ?)");

  this->track_insert = stmt_create(this,
    "INSERT OR ROLLBACK INTO tracks (filename) VALUES (?)");

  this->track_find = stmt_create(this,
    "SELECT DISTINCT tag_rels.track_id FROM tag_rels, tags "
    "WHERE tags.value MATCH ? AND tags.docid = tag_rels.tag_id");

  this->track_tags = stmt_create(this,
    "SELECT tags.key, tags.value FROM tags, tag_rels WHERE "
    "tag_rels.track_id = ? AND tags.docid = tag_rels.tag_id");
}

keeper_ptr keeper_create(const char *filename) {
  keeper_ptr this;

  CX_LOG("Creating keeper: %s", filename);
  database_file = cx_string_create(filename);

  this = keeper_raw_create();
  if (this == NULL) return NULL;

  // create tables
  if ((SQLITE_OK != keeper_exec(this,
      "CREATE TABLE IF NOT EXISTS tracks ("
        "id INTEGER PRIMARY KEY,"
        "filename TEXT UNIQUE"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE TABLE IF NOT EXISTS tag_rels ("
        "track_id INTEGER, tag_id INTEGER, UNIQUE(track_id, tag_id)"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE VIRTUAL TABLE IF NOT EXISTS tags USING fts4(key, value"
      ")")) || (SQLITE_OK != keeper_exec(this,
      "CREATE TABLE IF NOT EXISTS instances ("
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

  // can create statements only after tables are in place
  impl_init_statements(this->impl);

  return this;
}

keeper_ptr keeper_copy(keeper_ptr keeper) {
  CX_UNUSED(keeper);
  keeper_ptr this = keeper_raw_create();
  impl_init_statements(this->impl);
  return this;
}

cx_iterator_ptr keeper_track_find(keeper_ptr keeper, cx_string_ptr needle) {
  CX_UNUSED(keeper);
  CX_LOG_INFO("Search for tracks with \"%s\"", needle->string);
  CX_LOG_ERROR("Not implemented");
  return NULL;
}

void keeper_track_insert(keeper_ptr keeper, track_info_ptr track) {
  sqlite_ptr impl = (sqlite_ptr)keeper->impl;
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

  stmt_reset(impl->track_insert);
  stmt_bind_string(impl->track_insert, 0, track->filename);
  if (0 != stmt_exec(impl->track_insert)) {
    CX_LOG_INFO("Track \"%s\" already exists in database",
      track->filename->string);
    return;
  }

  track->id = sqlite3_last_insert_rowid(impl->db);
  CX_LOG_DEBUG("Track \"%s\" id = %d", track->filename, track->id);
  
  for (i = 0; i < tags_count(track->tags); ++i) {
    stmt_reset(impl->tags_insert);
    stmt_bind_string(impl->tags_insert, 0, tags_get(track->tags, i)->key);
    stmt_bind_string(impl->tags_insert, 1, tags_get(track->tags, i)->value);
    stmt_exec(impl->tags_insert);

    int tag_id = sqlite3_last_insert_rowid(impl->db);
    stmt_reset(impl->tag_rels_insert);
    stmt_bind_int(impl->tag_rels_insert, 0, track->id);
    stmt_bind_int(impl->tag_rels_insert, 1, tag_id);
    stmt_exec(impl->tag_rels_insert);
  }
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


