// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/tile_format_parser.hpp"

#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"

namespace tactile::yml_format {

auto parse_tile_encoding(const StringView name) -> Maybe<TileEncoding>
{
  if (name == "plain") {
    return TileEncoding::kPlainText;
  }
  else if (name == "base64") {
    return TileEncoding::kBase64;
  }

  return kNothing;
}

auto parse_compression_kind(const StringView name) -> Maybe<CompressionKind>
{
  if (name == "none") {
    return CompressionKind::kNone;
  }
  else if (name == "zlib") {
    return CompressionKind::kZlib;
  }
  else if (name == "zstd") {
    return CompressionKind::kZstd;
  }

  return kNothing;
}

auto parse_tile_format(const YAML::Node& root_node) -> Result<ir::TileFormat>
{
  ir::TileFormat tile_format {};
  tile_format.encoding = TileEncoding::kPlainText;
  tile_format.compression = CompressionMode::kNone;

  const auto tile_format_node = root_node["tile-format"];
  if (!tile_format_node) {
    return tile_format;
  }

  TACTILE_LOG_TRACE("Parsing tile format node at {}:{}",
                    tile_format_node.Mark().line,
                    tile_format_node.Mark().column);

  if (const auto encoding_name = try_parse<String>(tile_format_node, "encoding")) {
    if (const auto encoding = parse_tile_encoding(*encoding_name)) {
      tile_format.encoding = *encoding;
    }
    else {
      TACTILE_LOG_WARN("Ignoring invalid tile encoding '{}'", *encoding_name);
    }
  }

  if (const auto compression_name = try_parse<String>(tile_format_node, "compression")) {
    if (const auto compression = parse_compression_kind(*compression_name)) {
      tile_format.compression = *compression;
    }
    else {
      TACTILE_LOG_WARN("Ignoring invalid compression mode '{}'", *compression_name);
    }
  }

  if (const auto zlib_level =
          try_parse<int>(tile_format_node, "zlib-compression-level")) {
    tile_format.compression_level = zlib_level;
  }

  if (const auto zstd_level =
          try_parse<int>(tile_format_node, "zstd-compression-level")) {
    tile_format.compression_level = zstd_level;
  }

  if (tile_format.encoding == TileEncoding::kPlainText &&
      tile_format.compression != CompressionKind::kNone) {
    TACTILE_LOG_ERROR("Plain text tile data encoding cannot be used with compression");
    return unexpected(make_save_format_error(SaveFormatError::kInvalidTileFormat));
  }

  // TODO validate zlib level
  // TODO validate zstd level

  return tile_format;
}

}  // namespace tactile::yml_format
