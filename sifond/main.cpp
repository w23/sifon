#include "Meta_ffmpeg.h"
#include "DB_ram.h"
#include "Gather.h"
#include "Scatter.h"
#include "Server.h"

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: music_path host port" << std::endl;
    return EINVAL;
  }
  
  av_register_all();
  
  Meta_ffmpeg meta;
  //DB_soci db("sifondb");
  DB_ram db;
  
  Gather gather(db, meta, argv[1]);
  Scatter scatter(db);
  
  Server server(db, scatter, argv[2], std::stoi(argv[3]));
  return server.serve();
}