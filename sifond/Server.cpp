#include "Server.h"
#include "IDatabase.h"

Server::Server(const IDatabase &db, const std::string &hostname, int port)
: db_(db)
, ev_http_(ev_base_)
, ev_http_socket_(ev_http_, hostname, port) {
  evhttp_set_gencb(*ev_http_, any_request_cb, this);
  evhttp_set_cb(*ev_http_, "/look", s_cb_req_get, this);
}

Server::~Server() {}

int Server::serve() {
  return ev_base_.dispatch();
}

void Server::any_request_cb(struct evhttp_request *req, void *arg) {
  std::cerr << "any request" << std::endl;
  evhttp_send_reply(req, 200, "WHATEVSERE", NULL);
}

struct Uri {
  struct evhttp_uri *uri;
  Uri(const char *struri) : uri(evhttp_uri_parse(struri)) {}
  ~Uri() { evhttp_uri_free(uri); }
};

void Server::req_get(struct evhttp_request *req) {
  std::cerr << "get request " << evhttp_request_get_uri(req) << std::endl;
  Uri uri(evhttp_request_get_uri(req));
  
  struct evkeyvalq query;
  evhttp_parse_query_str(evhttp_uri_get_query((uri.uri)), &query);
  
  Tags tags;
  struct evkeyval *it;
  TAILQ_FOREACH(it, &query, next) {
	tags.emplace(it->key, it->value);
  }

  TrackInfo_v tracks(db_.find(tags));
  std::stringstream ss;
  ss << "[";
  bool first = true;
  for (auto t: tracks) {
    if (!first) ss << ","; else first = false;
    ss << "{";
    ss << "\"id\":" << t.track_id;
    ss << ",[";
    bool firstag = true;
    for (auto tag: t.tags) {
      if (!firstag) ss << ","; else firstag = false;
      
//////////////////////////////
      //! \todo escape!!!!111
      ss << "{\"" << tag.first << "\":\"" << tag.second << "\"}";
//////////////////////////////
    }
    ss << "]}";
  }
  ss << "]";

  evbuffer *evb = evbuffer_new();
  std::string str(ss.str());
  evbuffer_add(evb, str.c_str(), str.length());
  evhttp_send_reply(req, 200, "OK", evb);
  evbuffer_free(evb);
  evhttp_clear_headers(&query);
}