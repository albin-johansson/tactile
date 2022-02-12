#include "compression.hpp"

#include <zlib.h>

#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr usize _buffer_size = 32'768;

using process_function = int (*)(z_stream*, int);
using end_function = int (*)(z_stream*);

[[nodiscard]] auto process_data(z_stream& stream,
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

}  // namespace

auto compress_with_zlib(const void* data, const usize bytes) -> maybe<zlib_data>
{
  TACTILE_ASSERT(data);
  z_stream stream{};

  if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
    return nothing;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return process_data(stream, deflate, deflateEnd, Z_FINISH);
}

auto decompress_with_zlib(const void* data, const usize bytes) -> maybe<zlib_data>
{
  TACTILE_ASSERT(data);
  z_stream stream{};

  if (inflateInit(&stream) != Z_OK) {
    return nothing;
  }

  stream.next_in = (Bytef*) data;
  stream.avail_in = static_cast<uInt>(bytes);

  return process_data(stream, inflate, inflateEnd, 0);
}

}  // namespace tactile
