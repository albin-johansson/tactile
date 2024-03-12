// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tile_format.hpp"

#include <zlib.h>

#include "tactile/core/debug/exception.hpp"

namespace tactile {

// See https://github.com/facebook/zstd/issues/3133 for Zstd compression levels

void TileFormat::set_encoding(const TileEncoding encoding)
{
  mEncoding = encoding;

  if (mEncoding == TileEncoding::Plain) {
    mCompression = TileCompression::None;
  }
}

void TileFormat::set_compression(const TileCompression compression)
{
  if (can_use_compression_strategy(compression)) {
    mCompression = compression;
  }
  else {
    throw Exception {"Invalid encoding/compression combination!"};
  }
}

void TileFormat::set_zlib_compression_level(const int level)
{
  if (is_valid_zlib_compression_level(level)) {
    mZlibCompressionLevel = level;
  }
  else {
    throw Exception {"Invalid Zlib compression level!"};
  }
}

void TileFormat::set_zstd_compression_level(const int level)
{
  if (is_valid_zstd_compression_level(level)) {
    mZstdCompressionLevel = level;
  }
  else {
    throw Exception {"Invalid zstd compression level!"};
  }
}

auto TileFormat::encoding() const -> TileEncoding
{
  return mEncoding;
}

auto TileFormat::compression() const -> TileCompression
{
  return mCompression;
}

auto TileFormat::zlib_compression_level() const -> int
{
  return mZlibCompressionLevel;
}

auto TileFormat::zstd_compression_level() const -> int
{
  return mZstdCompressionLevel;
}

auto TileFormat::supports_any_compression() const -> bool
{
  return mEncoding != TileEncoding::Plain;
}

auto TileFormat::can_use_compression_strategy(const TileCompression compression) const
    -> bool
{
  return compression == TileCompression::None || mEncoding != TileEncoding::Plain;
}

auto TileFormat::is_valid_zlib_compression_level(const int level) -> bool
{
  return level == Z_DEFAULT_COMPRESSION ||
         (level >= min_zlib_compression_level() && level <= max_zlib_compression_level());
}

auto TileFormat::is_valid_zstd_compression_level(const int level) -> bool
{
  return level >= min_zstd_compression_level() && level <= max_zstd_compression_level();
}

auto TileFormat::min_zlib_compression_level() -> int
{
  return Z_BEST_SPEED;
}

auto TileFormat::max_zlib_compression_level() -> int
{
  return Z_BEST_COMPRESSION;
}

auto TileFormat::min_zstd_compression_level() -> int
{
  return 1;
}

auto TileFormat::max_zstd_compression_level() -> int
{
  return 19;
}

}  // namespace tactile
