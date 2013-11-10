#include "Server.h"
#include "IDatabase.h"
#include "Scatter.h"

struct Uri {
  struct evhttp_uri *uri;
  Uri(const char *struri) : uri(evhttp_uri_parse(struri)) {}
  ~Uri() { evhttp_uri_free(uri); }
};


Server::Server(const IDatabase &db, const Scatter &scatter, const std::string &hostname, int port)
: db_(db), scatter_(scatter)
, ev_http_(ev_base_)
, ev_http_socket_(ev_http_, hostname, port) {
  
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    std::cerr << "failed to ignore SIGPIPE" << std::endl;
  
  evhttp_set_gencb(*ev_http_, s_cb_req, this);
  evhttp_set_cb(*ev_http_, "/look", s_cb_req_look, this);
}

Server::~Server() {}

int Server::serve() {
  return ev_base_.dispatch();
}

void Server::req(struct evhttp_request *req) {
  std::cerr << "any request" << std::endl;
  Uri uri(evhttp_request_get_uri(req));

  std::string rawpath(evhttp_uri_get_path(uri.uri));
  std::vector<std::string> path;
  boost::split(path, rawpath, boost::is_any_of("/"));
  
  auto it = std::remove_if(begin(path), end(path), [](const std::string &str){
    return str.empty();
  });
  path.erase(it, path.end());
  
  if (path.size() > 1) {
    if (path.size() == 3 && path[0] == "listen") {
      req_listen(req, std::stoll(path[1]), path[2]);
      return;
    }
  }
  
  evhttp_send_reply(req, 404, "WHAT", NULL);
}

void Server::req_look(struct evhttp_request *req) {
  std::cerr << "look request " << evhttp_request_get_uri(req) << std::endl;
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

void Server::req_listen(struct evhttp_request *req,
                        std::int64_t track_id, const std::string &view) {
  IStream *stream = scatter_.get_track_view(track_id, view);
  
  if (!stream) {
    evhttp_send_reply(req, 404, "NASHI", NULL);
    return;
  }
  
  evhttp_add_header(evhttp_request_get_output_headers(req),
                   "Content-Type", stream->mime_type().c_str());
  evhttp_send_reply_start(req, 200, "OK");
  evbuffer *chunkbuffer = evbuffer_new();
  for (;;) {
    IStream::packet_t pkt(stream->get_next_packet());
    if (!pkt.size) break;
    evbuffer_add(chunkbuffer, pkt.data, pkt.size);
    evhttp_send_reply_chunk(req, chunkbuffer);
  }
  evbuffer_free(chunkbuffer);
  evhttp_send_reply_end(req);
}