// Copyright (C) 2025 Albin Johansson

module;

#define Z_PREFIX_SET
#include <zlib.h>

module tactile.zlib;

import std;

namespace tactile {
namespace {

// Zlib has inconsistently named typedefs, we use these instead.
using z_byte = ::Bytef;
using z_uint = ::uInt;
using z_ulong = ::uLong;

/// Type used for staging buffers used to batch data processing.
using StagingBuffer = Array<z_byte, 2'048uz>;

/// Provides callbacks that controls the behavior of stream processing
/// functions.
struct ZlibCallbacks final
{
  using start_fn = int (*)(z_stream*);
  using estimate_output_buffer_size_fn = usize (*)(z_stream*, usize);
  using update_fn = int (*)(z_stream*, int);
  using finish_fn = int (*)(z_stream*);

  start_fn start {};
  estimate_output_buffer_size_fn estimate_output_buffer_size {};
  update_fn update {};
  finish_fn finish {};
};

/// Initializes a Zlib stream.
[[nodiscard]]
auto zlib_start(z_stream& stream,
                const ZlibCallbacks& callbacks,
                const Span<const u8> input_data,
                StagingBuffer& staging_buffer) -> Result<void>
{
  stream.next_in = const_cast<z_byte*>(input_data.data());  // NOLINT
  stream.avail_in = checked_cast<z_uint>(input_data.size_bytes());
  stream.next_out = staging_buffer.data();
  stream.avail_out = checked_cast<z_uint>(staging_buffer.size());

  const auto init_stream_result = callbacks.start(&stream);
  if (init_stream_result != Z_OK) {
    get_logger().log(LogLevel::kError,
                     "Could not initialize z_stream: {}",
                     zError(init_stream_result));
    return err(Error::kInvalidOp);
  }

  return ok();
}

/// Processes a stream in chunks.
[[nodiscard]]
auto zlib_process(z_stream& stream,
                  const ZlibCallbacks& callbacks,
                  StagingBuffer& staging_buffer,
                  Vector<u8>& output_buffer) -> Result<void>
{
  const auto copy_processed_batch_to_output_buffer = [&] {
    const auto written_bytes = staging_buffer.size() - stream.avail_out;
    output_buffer.insert(output_buffer.end(),
                         staging_buffer.data(),
                         staging_buffer.data() + written_bytes);
  };

  while (true) {
    const auto update_result = callbacks.update(&stream, Z_FINISH);

    if (update_result == Z_STREAM_END) {
      copy_processed_batch_to_output_buffer();
      break;
    }

    if (update_result == Z_OK || update_result == Z_BUF_ERROR) {
      // We ran out of space in the staging buffer, so we need to flush and
      // reuse it.
      copy_processed_batch_to_output_buffer();
      stream.next_out = staging_buffer.data();
      stream.avail_out = checked_cast<z_uint>(staging_buffer.size());
    }
    else {
      get_logger().log(LogLevel::kError,
                       "Could not process Zlib chunk: {}",
                       zError(update_result));
      return err(Error::kUnknown);
    }
  }

  return ok();
}

/// Finalizes a Zlib stream.
[[nodiscard]]
auto zlib_finish(z_stream& stream, const ZlibCallbacks& callbacks)
    -> Result<void>
{
  const auto end_stream_result = callbacks.finish(&stream);

  if (end_stream_result != Z_OK) {
    get_logger().log(LogLevel::kError,
                     "Could not finalize z_stream: {}",
                     zError(end_stream_result));
    return err(Error::kUnknown);
  }

  return ok();
}

/// Runs Zlib on a given stream of bytes.
[[nodiscard]]
auto zlib_apply(const Span<const u8> data, const ZlibCallbacks& callbacks)
    -> Result<Vector<u8>>
{
  z_stream stream {};
  StagingBuffer staging_buffer {};
  Vector<u8> output_buffer {};

  return zlib_start(stream, callbacks, data, staging_buffer)
      .and_then([&] {
        const auto output_buffer_size =
            callbacks.estimate_output_buffer_size(&stream, data.size());
        output_buffer.reserve(output_buffer_size);
        return zlib_process(stream, callbacks, staging_buffer, output_buffer);
      })
      .and_then([&] { return zlib_finish(stream, callbacks); })
      .transform([&] { return std::move(output_buffer); });
}

}  // namespace

auto ZlibCompressionFormat::compress(const Span<const u8> data) const
    -> Result<Vector<u8>>
{
  ZlibCallbacks callbacks {};
  callbacks.start = [](z_stream* stream) {
    return z_deflateInit(stream, Z_DEFAULT_COMPRESSION);
  };
  callbacks.estimate_output_buffer_size = [](z_stream* stream,
                                             const usize data_size) -> usize {
    return deflateBound(stream, checked_cast<z_ulong>(data_size));
  };
  callbacks.update = &deflate;
  callbacks.finish = &deflateEnd;

  return zlib_apply(data, callbacks);
}

auto ZlibCompressionFormat::decompress(const Span<const u8> data) const
    -> Result<Vector<u8>>
{
  ZlibCallbacks callbacks {};
  callbacks.start = [](z_stream* stream) { return z_inflateInit(stream); };
  callbacks.estimate_output_buffer_size =
      [](z_stream*, const usize data_size) -> usize { return data_size * 2uz; };
  callbacks.update = &inflate;
  callbacks.finish = &inflateEnd;

  return zlib_apply(data, callbacks);
}

}  // namespace tactile
