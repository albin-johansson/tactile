// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/tile_format_emitter.hpp"

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/tactile-yml-format/emit/stream.hpp"

namespace tactile::yml_format {

void emit(YAML::Emitter& emitter, const TileEncoding tile_encoding)
{
  switch (tile_encoding) {
    case TileEncoding::kPlainText: emitter << "plain"; break;
    case TileEncoding::kBase64: emitter << "base64"; break;
    default: throw Exception {"Did not recognize tile encoding"};
  }
}

void emit(YAML::Emitter& emitter, const CompressionKind compression_kind)
{
  switch (compression_kind) {
    case CompressionMode::kNone: emitter << "none"; break;
    case CompressionMode::kZlib: emitter << "zlib"; break;
    case CompressionMode::kZstd: emitter << "zstd"; break;
    default: throw Exception {"Did not recognize compression mode"};
  }
}

void emit_tile_format(YAML::Emitter& emitter, const ir::TileFormat& format)
{
  if (format.encoding == TileEncoding::kPlainText) {
    return;
  }

  emitter << YAML::Key << "tile-format" << YAML::BeginMap;
  emitter << YAML::Key << "encoding" << YAML::Value << format.encoding;

  if (format.compression != CompressionMode::kNone) {
    emitter << YAML::Key << "compression" << YAML::Value << format.compression;
  }

  if (format.compression == CompressionMode::kZlib &&
      format.compression_level.has_value() && format.compression_level != -1) {
    emitter << YAML::Key << "zlib-compression-level" << YAML::Value
            << *format.compression_level;
  }

  if (format.compression == CompressionMode::kZstd &&
      format.compression_level.has_value()) {
    emitter << YAML::Key << "zstd-compression-level" << YAML::Value
            << *format.compression_level;
  }

  emitter << YAML::EndMap;
}

}  // namespace tactile::yml_format
