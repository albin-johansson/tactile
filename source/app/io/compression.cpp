#include "compression.hpp"

#include <algorithm>  // clamp, copy_n

#include <folly/compression/Zlib.h>
#include <folly/compression/Zstd.h>
#include <folly/io/IOBuf.h>
#include <zlib.h>
#include <zstd.h>

namespace tactile::io {
namespace {

[[nodiscard]] auto to_byte_stream(const folly::IOBuf& buf) -> ByteStream
{
  ByteStream stream;
  stream.resize(buf.length());

  std::copy_n(buf.data(), buf.length(), stream.data());
  return stream;
}

}  // namespace

auto zlib_compress(const void* source, const usize source_bytes, int level)
    -> Maybe<ByteStream>
{
  if (!source || source_bytes == 0) {
    return nothing;
  }

  if (level != Z_DEFAULT_COMPRESSION) {
    level = std::clamp(level, Z_BEST_SPEED, Z_BEST_COMPRESSION);
  }

  folly::IOBuf buf {folly::IOBuf::WRAP_BUFFER, source, source_bytes};

  auto codec = folly::io::zlib::getCodec(folly::io::zlib::Options(), level);
  const auto compressed = codec->compress(&buf);

  return to_byte_stream(*compressed);
}

auto zlib_decompress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
{
  if (!source) {
    return nothing;
  }

  folly::IOBuf buf {folly::IOBuf::WRAP_BUFFER, source, source_bytes};
  const auto uncompressed = folly::io::zlib::getCodec()->uncompress(&buf);

  return to_byte_stream(*uncompressed);
}

auto zlib_decompress(std::span<const uint8> span) -> Maybe<ByteStream>
{
  return zlib_decompress(span.data(), span.size_bytes());
}

auto zstd_compress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
{
  using folly::io::zstd::getCodec;
  using folly::io::zstd::Options;

  if (!source || source_bytes == 0) {
    return nothing;
  }

  folly::IOBuf buf {folly::IOBuf::WRAP_BUFFER, source, source_bytes};
  const auto compressed = getCodec(Options {ZSTD_CLEVEL_DEFAULT})->compress(&buf);

  return to_byte_stream(*compressed);
}

auto zstd_decompress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
{
  using folly::io::zstd::getCodec;
  using folly::io::zstd::Options;

  if (!source) {
    return nothing;
  }

  folly::IOBuf buf {folly::IOBuf::WRAP_BUFFER, source, source_bytes};
  const auto uncompressed = getCodec(Options {ZSTD_CLEVEL_DEFAULT})->uncompress(&buf);

  return to_byte_stream(*uncompressed);
}

auto zstd_decompress(std::span<const uint8> span) -> Maybe<ByteStream>
{
  return zstd_decompress(span.data(), span.size_bytes());
}

}  // namespace tactile::io
