// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/compression/zlib_compression_provider.hpp"

#include <algorithm>  // clamp
#include <utility>    // move

#define Z_PREFIX_SET
#include <zlib.h>

#include "tactile/foundation/container/array.hpp"
#include "tactile/foundation/debug/profiler.hpp"
#include "tactile/foundation/functional/function.hpp"
#include "tactile/foundation/io/compression/compression_error.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {
namespace {

// Use 32 KB for temporary stack buffers.
inline constexpr usize kBufferSize = 32'768;

using StackBuffer = Array<Bytef, kBufferSize>;

struct ZlibCallbacks final {
  Function<int(z_stream*)> init_stream;
  Function<int(z_stream*, int)> process;
  Function<int(z_stream*)> end_stream;
  Function<ByteStream(z_stream*)> make_byte_stream;
};

[[nodiscard]]
auto _process_chunks(const ZlibCallbacks& callbacks,
                     z_stream& stream,
                     StackBuffer& out_buffer) -> Result<ByteStream>
{
  auto byte_stream = callbacks.make_byte_stream(&stream);

  const auto copy_processed_batch_to_byte_stream = [&] {
    const auto written_bytes = sizeof out_buffer - stream.avail_out;
    byte_stream.insert(byte_stream.end(),
                       out_buffer.data(),
                       out_buffer.data() + written_bytes);
  };

  while (true) {
    const auto process_result = callbacks.process(&stream, Z_FINISH);
    if (process_result == Z_STREAM_END) {
      copy_processed_batch_to_byte_stream();
      break;
    }
    else if (process_result == Z_OK || process_result == Z_BUF_ERROR) {
      TACTILE_LOG_TRACE("[ZlibCompressionProvider] Flushing and resetting output buffer");

      // We ran out of space in the output buffer, so reuse it!
      copy_processed_batch_to_byte_stream();
      stream.next_out = out_buffer.data();
      stream.avail_out = static_cast<uInt>(out_buffer.size());
    }
    else {
      TACTILE_LOG_ERROR("Could not process Zlib chunk: {}", zError(process_result));
      return unexpected(make_compression_error(CompressionError::kInternalError));
    }
  }

  return byte_stream;
}

[[nodiscard]]
auto _process_data(const ZlibCallbacks& callbacks, const ByteSpan input_data)
    -> Result<ByteStream>
{
  StackBuffer out_buffer;

  z_stream stream {};
  stream.next_in = (Bytef*) input_data.data();  // NOLINT: not much we can do here.
  stream.avail_in = static_cast<uInt>(input_data.size_bytes());
  stream.next_out = out_buffer.data();
  stream.avail_out = sizeof out_buffer;

  if (const auto init_result = callbacks.init_stream(&stream); init_result != Z_OK) {
    TACTILE_LOG_ERROR("Could not initialize z_stream: {}", zError(init_result));
    return unexpected(make_compression_error(CompressionError::kInternalError));
  }

  return _process_chunks(callbacks, stream, out_buffer)
      .and_then([&](ByteStream byte_stream) -> Result<ByteStream> {
        if (const auto end_result = callbacks.end_stream(&stream); end_result != Z_OK) {
          TACTILE_LOG_ERROR("Could not finalize z_stream: {}", zError(end_result));
          return unexpected(make_compression_error(CompressionError::kInternalError));
        }

        return std::move(byte_stream);
      });
}

}  // namespace

auto ZlibCompressionProvider::compress(const ByteSpan data) const -> Result<ByteStream>
{
  TACTILE_DEBUG_PROFILE_SCOPE("ZlibCompressionProvider::compress");

  if (data.empty()) {
    return unexpected(make_compression_error(CompressionError::kNoData));
  }

  const auto level = mLevel.value_or(Z_DEFAULT_COMPRESSION);

  ZlibCallbacks callbacks;
  callbacks.init_stream = [=](z_stream* stream) { return z_deflateInit(stream, level); };
  callbacks.process = &deflate;
  callbacks.end_stream = &deflateEnd;

  callbacks.make_byte_stream = [data](z_stream* stream) {
    ByteStream byte_stream;

    const auto bound = deflateBound(stream, static_cast<uLong>(data.size_bytes()));
    byte_stream.reserve(static_cast<usize>(bound));

    return byte_stream;
  };

  return _process_data(callbacks, data);
}

auto ZlibCompressionProvider::decompress(const ByteSpan data) const -> Result<ByteStream>
{
  TACTILE_DEBUG_PROFILE_SCOPE("ZlibCompressionProvider::decompress");

  if (data.empty()) {
    return unexpected(make_compression_error(CompressionError::kNoData));
  }

  ZlibCallbacks callbacks;
  callbacks.init_stream = [](z_stream* stream) { return z_inflateInit(stream); };
  callbacks.process = &inflate;
  callbacks.end_stream = &inflateEnd;

  callbacks.make_byte_stream = [](z_stream* /* stream */) {
    ByteStream byte_stream;
    byte_stream.reserve(2'048);
    return byte_stream;
  };

  return _process_data(callbacks, data);
}

void ZlibCompressionProvider::set_compression_level(const Maybe<int> level)
{
  if (level.has_value()) {
    mLevel = std::clamp(*level, min_compression_level(), max_compression_level());
  }
  else {
    mLevel.reset();
  }
}

auto ZlibCompressionProvider::get_compression_level() const -> Maybe<int>
{
  return mLevel;
}

auto ZlibCompressionProvider::min_compression_level() -> int
{
  return Z_BEST_SPEED;
}

auto ZlibCompressionProvider::max_compression_level() -> int
{
  return Z_BEST_COMPRESSION;
}

}  // namespace tactile