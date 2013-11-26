#include "Scatter.h"
#include "IDatabase.h"

template <int BUFSZ>
class FileStream : public IStream {
public:
  FileStream(const std::string &filename) {
    //file_.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file_.open(filename, std::ios::binary);
    if (!file_)
      throw std::runtime_error("cannot open file " + filename);
  };
  virtual ~FileStream() {}
  virtual std::string mime_type() const { return "application/octet-stream"; }
  virtual packet_t get_next_packet() {
    file_.read(buffer_, BUFSZ);
    return {buffer_, static_cast<size_t>(file_.gcount())};
  }
  
private:
  std::ifstream file_;
  char buffer_[BUFSZ];
};

Scatter::Scatter(IDatabase &db) : db_(db) {
  
}

void Scatter::add_preset(const std::string &preset, IEncoder *encoder) {
  if (preset == "origin" || presets_.count(preset) != 0)
    throw std::runtime_error("invalid preset name \"" + preset + "\"");
  
  presets_.insert(std::make_pair(preset, std::unique_ptr<IEncoder>(encoder)));
}

IStream *Scatter::get_track_view(std::int64_t track_id,
                                 const std::string &view) const {
  TrackView_v views(db_.views(TrackInfo(track_id)));
  auto v = std::find_if(views.begin(), views.end(),
                        [=](const TrackView &v){ return v.preset == view; });
  
  if (v != views.end())
    return new FileStream<65536>(v->filename);
  
  if (views.empty())
    return nullptr;
  
  auto orig = std::find_if(views.begin(), views.end(),
                          [=](const TrackView &v){ return v.preset == "origin"; });
  if (orig == views.end()) {
    std::cerr << "Track " + std::to_string(track_id)
      + " is without origin? wtf!" << std::endl;
    return nullptr;
  }
  
  //! \todo encode
  //auto pres =
  return nullptr;
}
