#include "Meta_ffmpeg.h"
#include "DB_ram.h"
#include "Gather.h"
#include "Server.h"

int main(int argc, char *argv[]) {
  av_register_all();
  
  Meta_ffmpeg meta;
  //DB_soci db("sifondb");
  DB_ram db;
  
  Gather gather(db, meta, argv[1]);
  
  Server server(db, "localhost", 8080);
  return server.serve();
}