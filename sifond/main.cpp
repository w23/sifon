#include "Server.h"

int main(int argc, char *argv[]) {
  Server server("localhost", 8080);
  return server.serve();
}