#include "Meta_ffmpeg.h"
#include "Gather.h"
#include "Server.h"

int main(int argc, char *argv[]) {
  av_register_all();
  
  Meta_ffmpeg meta;
  
  Gather gather(nullptr, &meta, argv[1]);
  
  Server server("localhost", 8080);
  return server.serve();
}