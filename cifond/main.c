#include "extern.h"
#include "cx_log.h"
#include "keeper.h"
#include "sourcer.h"
#include "instancer.h"
#include "server.h"

int main(int argc, char *argv[]) {
  const char *storage;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path_to_scan>\n", argv[0]);
    return 1;
  }

  storage = argv[1];
  cx_log_level = CX_LOG_LEVEL_DEBUG;

  CX_LOG("################");
  CX_LOG("Staring cifond with media source dir %s", storage);

  av_register_all();

  keeper_ptr keeper = keeper_create("/tmp/sifon.sqlite");

  sourcer_start(keeper, storage);
  instancer_start();

  int result = server_run(NULL, 8080);

  instancer_stop();
  sourcer_stop();
  cx_release(keeper);

  return result;
}
