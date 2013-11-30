#include "extern.h"
#include "cx_log.h"
#include "server.h"

int server_run(const char *bind_host, int port) {
  CX_LOG("Listening for HTTP requests on %s:%d",
    (bind_host != NULL)?bind_host:"*", port);
  CX_LOG_ERROR("Not implemented");
  return 1;
}
