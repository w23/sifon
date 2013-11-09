#pragma once
#include "stable.h"

class IDatabase;
class Server {
public:
  Server(const IDatabase &db, const std::string &hostname, int port);
  ~Server();
  
  int serve();
  
private:
  static void any_request_cb(struct evhttp_request *req, void *arg);
  //static void s_cb_req_play(struct evhttp_request *req, void *arg);
  
  inline static void s_cb_req_get(struct evhttp_request *req, void *arg) {
    reinterpret_cast<Server*>(arg)->req_get(req);
  }
  void req_get(struct evhttp_request *req);
  
  const IDatabase &db_;
  
  class EvBase {
    struct event_base *base_;
  public:
    inline EvBase() : base_(event_base_new()) {
      if (!base_) throw std::runtime_error("cannot create libevent base");
    }
    inline ~EvBase() {
      event_base_free(base_);
    }
    inline struct event_base *operator*() { return base_; }
    inline int dispatch() { return event_base_dispatch(base_); }
  } ev_base_; // class EvBase
  
  class EvHttp {
    struct evhttp *http_;
  public:
    inline EvHttp(EvBase &base) : http_(evhttp_new(*base)) {
      if (!http_) throw std::runtime_error("cannot create libevent http");
    }
    inline ~EvHttp() {
      evhttp_free(http_);
    }
    inline struct evhttp *operator*() { return http_; }
  } ev_http_; // class EvHttp
  
  class EvHttpSocket {
    struct evhttp *http_;
    struct evhttp_bound_socket *socket_;
  public:
    inline EvHttpSocket(EvHttp &http, const std::string &hostname, int port)
    : http_(*http)
    , socket_(evhttp_bind_socket_with_handle(http_, hostname.c_str(), port)) {
      if (!socket_) throw std::runtime_error("cannot bind libevent http");
    }
    inline ~EvHttpSocket() {
      evhttp_del_accept_socket(http_, socket_);
    }
  } ev_http_socket_; // class EvHttpSocket
};