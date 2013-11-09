#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>
#include <sys/queue.h> // TAILQ

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>