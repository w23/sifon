#include "extern.h"
#include <signal.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>
#include <sys/queue.h> // TAILQ
#include "cx_log.h"
#include "keeper.h"
#include "server.h"

struct event_base *ev_base;
struct evhttp *ev_http;
struct evhttp_bound_socket *ev_socket;
keeper_ptr keeper;

static void request_search(struct evhttp_request *req, const char *needle) {
  CX_LOG_INFO("Requset: search for \"%s\"", needle);
  cx_string_ptr sneedle = cx_string_create(needle);
  cx_array_ptr tracks = keeper_track_find(keeper, sneedle, 0, 64);
  cx_release(sneedle);

  evhttp_add_header(evhttp_request_get_output_headers(req),
                    "Content-Type", "application/json");
  struct evbuffer *evb = evbuffer_new();
  evbuffer_add(evb, "[", 1);

  for (unsigned i = 0; i < tracks->count; ++i) {
    track_info_ptr track = CX_ARRAY_AT(tracks, i, track_info);
    evbuffer_add_printf(evb, "{\"id\":%d,\"tags\":{", track->id);
    for (unsigned j = 0; j < tags_count(track->tags); ++j) {
      tag_ptr tag = tags_get(track->tags, j);
      evbuffer_add_printf(evb, "%c\"%s\":\"%s\"", (j==0)?' ':',',
        tag->key->string, tag->value->string);
    }
    evbuffer_add(evb, "}", 1);
  }

  evbuffer_add(evb, "]", 1);

  evhttp_send_reply(req, 200, "OK", evb);

  evbuffer_free(evb);
  cx_release(tracks);
}

static void callback_http_request(struct evhttp_request *req, void *arg) {
  CX_UNUSED(arg);
  CX_LOG_INFO("HTTP request: %s", evhttp_request_get_uri(req));
  const char *path = evhttp_request_get_uri(req);
  if (strncmp(path, "/search/", 8) == 0) {
    request_search(req, path + 8);
    return;
  }
}

int server_run(keeper_ptr inkeeper, const char *bind_host, int port) {
  keeper = inkeeper;
  CX_LOG("Listening for HTTP requests on %s:%d",
    (bind_host != NULL)?bind_host:"*", port);

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    CX_LOG_WARNING("failed to ignore SIGPIPE");

  ev_base = event_base_new();
  if (ev_base == NULL) {
    CX_LOG_ERROR("Cannot create libevent base");
    return -1;
  }

  ev_http = evhttp_new(ev_base);
  if (ev_http == NULL) {
    CX_LOG_ERROR("Cannot create libevent http");
    event_base_free(ev_base);
    return -1;
  }

  ev_socket = evhttp_bind_socket_with_handle(ev_http, bind_host, port);
  if (ev_socket == NULL) {
    CX_LOG_ERROR("Cannot bind libevent http socket");
    evhttp_free(ev_http);
    event_base_free(ev_base);
    return -1;
  }

  evhttp_set_gencb(ev_http, callback_http_request, NULL);

  int result = event_base_dispatch(ev_base);

  evhttp_del_accept_socket(ev_http, ev_socket);
  evhttp_free(ev_http);
  event_base_free(ev_base);
  return result;
}
