#include "compression.hpp"

#include <spdlog/spdlog.h>
#include <zlib.h>

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

constexpr usize _buffer_size = 32'768;

using ProcessFunction = int (*)(z_stream*, int);
using EndFunction = int (*)(z_stream*);

[[nodiscard]] auto _process_data(z_stream&       stream,
                                 ProcessFunction process,
                                 EndFunction     end,
                                 const int       flush) -> std::optional<ZlibData>
{
  ZlibData out;
  out.reserve(128);

  Bytef buffer[_buffer_size];
  int   status {};

  do {
    stream.next_out = buffer;
    stream.avail_out = sizeof buffer;

    status = process(&stream, flush);

    if (out.size() < stream.total_out) {
      const auto count = stream.total_out - out.size();
      for (usize idx = 0; idx < count; ++idx) {
        out.push_back(buffer[idx]);
      }
    }
  } while (status == Z_OK);

  end(&stream);

  if (status != Z_STREAM_END) {
    return std::nullopt;
  }

  return out;
}

[[nodiscard]] auto _convert(const ZlibCompressionLevel level) -> int
{
  switch (level) {
    case ZlibCompressionLevel::Default:
      return Z_DEFAULT_COMPRESSION;

    case ZlibCompressionLevel::BestCompression:
      return Z_BEST_COMPRESSION;

    case ZlibCompressionLevel::BestSpeed:
      return Z_BEST_SPEED;

    default:
      throw TactileError {"Invalid Zlib compression level!"};
  }
}

}  // namespace

auto compress_with_zlib(const void*                data,
                        const usize                bytes,
                        const ZlibCompressionLevel level) -> std::optional<ZlibData>
{
  TACTILE_ASSERT(data);
  z_stream stream {};

  if (deflateInit(&stream, _convert(level)) != Z_OK) {
    spdlog::error("Could not initialize z_stream for data compression!");
    return std::nullopt;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return _process_data(stream, deflate, deflateEnd, Z_FINISH);
}

auto decompress_with_zlib(const void* data, const usize bytes) -> std::optional<ZlibData>
{
  TACTILE_ASSERT(data);
  z_stream stream {};

  if (inflateInit(&stream) != Z_OK) {
    spdlog::error("Could not initialize z_stream for data decompression!");
    return std::nullopt;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return _process_data(stream, inflate, inflateEnd, 0);
}

}  // namespace tactile
