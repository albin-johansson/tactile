#include "compression.hpp"

#include <zlib.h>

#include "misc/assert.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"

namespace tactile {
namespace {

constexpr usize _buffer_size = 32'768;

using process_function = int (*)(z_stream*, int);
using end_function = int (*)(z_stream*);

[[nodiscard]] auto _process_data(z_stream& stream,
                                 process_function process,
                                 end_function end,
                                 const int flush) -> maybe<zlib_data>
{
  zlib_data out;
  out.reserve(128);

  Bytef buffer[_buffer_size];
  int status{};

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
    return nothing;
  }

  return out;
}

[[nodiscard]] auto _convert(const zlib_compression_level level) -> int
{
  switch (level) {
    case zlib_compression_level::standard:
      return Z_DEFAULT_COMPRESSION;

    case zlib_compression_level::best_compression:
      return Z_BEST_COMPRESSION;

    case zlib_compression_level::best_speed:
      return Z_BEST_SPEED;

    default:
      throw_traced(tactile_error{"Invalid Zlib compression level!"});
  }
}

}  // namespace

auto compress_with_zlib(const void* data,
                        const usize bytes,
                        const zlib_compression_level level) -> maybe<zlib_data>
{
  TACTILE_ASSERT(data);
  z_stream stream{};

  if (deflateInit(&stream, _convert(level)) != Z_OK) {
    log_error("Could not initialize z_stream for data compression!");
    return nothing;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return _process_data(stream, deflate, deflateEnd, Z_FINISH);
}

auto decompress_with_zlib(const void* data, const usize bytes) -> maybe<zlib_data>
{
  TACTILE_ASSERT(data);
  z_stream stream{};

  if (inflateInit(&stream) != Z_OK) {
    log_error("Could not initialize z_stream for data decompression!");
    return nothing;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return _process_data(stream, inflate, inflateEnd, 0);
}

}  // namespace tactile
