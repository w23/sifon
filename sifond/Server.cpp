#include "Server.h"
#include <stdexcept>

Server::Server(const std::string &hostname, int port)
: ev_http_(ev_base_)
, ev_http_socket_(ev_http_, hostname, port) {
  evhttp_set_gencb(*ev_http_, any_request_cb, this);
}

Server::~Server() {}

int Server::serve() {
  return ev_base_.dispatch();
}

void Server::any_request_cb(struct evhttp_request *req, void *arg) {
  evhttp_send_reply(req, 200, "WHATEVSERE", NULL);
}
