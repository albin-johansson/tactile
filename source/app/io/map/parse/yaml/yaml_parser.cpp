/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "yaml_parser.hpp"

#include <yaml-cpp/yaml.h>

#include "io/util/yaml.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_tile_format(const YAML::Node& node, ir::TileFormatData& format)
    -> ParseError
{
  if (auto encoding = node["encoding"]) {
    auto encoding_str = encoding.as<std::string>();
    if (encoding_str == "plain") {
      format.encoding = TileEncoding::Plain;
    }
    else if (encoding_str == "base64") {
      format.encoding = TileEncoding::Base64;
    }
    else {
      return ParseError::BadTileFormatEncoding;
    }
  }
  else {
    format.encoding = TileEncoding::Plain;
  }

  if (auto compression = node["compression"]) {
    auto compression_str = compression.as<std::string>();
    if (compression_str == "none") {
      format.compression = TileCompression::None;
    }
    else if (compression_str == "zlib") {
      format.compression = TileCompression::Zlib;
    }
    else if (compression_str == "zstd") {
      format.compression = TileCompression::Zstd;
    }
    else {
      return ParseError::BadTileFormatCompression;
    }
  }
  else {
    format.compression = TileCompression::None;
  }

  read_attribute(node, "zlib-compression-level", format.zlib_compression_level, -1);

  if (format.encoding == TileEncoding::Plain &&
      format.compression != TileCompression::None) {
    return ParseError::PlainEncodingWithCompression;
  }

  if (const auto level = format.zlib_compression_level;
      level && !TileFormat::is_valid_zlib_compression_level(*level)) {
    return ParseError::BadZlibCompressionLevel;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_map(const fs::path& path, ir::MapData& data) -> ParseError
{
  const auto node = YAML::LoadFile(path.string());
  if (!node) {
    return ParseError::CouldNotReadFile;
  }

  if (!read_attribute(node, "row-count", data.row_count)) {
    return ParseError::NoMapHeight;
  }

  if (!read_attribute(node, "column-count", data.col_count)) {
    return ParseError::NoMapWidth;
  }

  if (!read_attribute(node, "tile-width", data.tile_size.x)) {
    return ParseError::NoMapTileWidth;
  }

  if (!read_attribute(node, "tile-height", data.tile_size.y)) {
    return ParseError::NoMapTileHeight;
  }

  if (!read_attribute(node, "next-layer-id", data.next_layer_id)) {
    return ParseError::NoMapNextLayerId;
  }

  if (!read_attribute(node, "next-object-id", data.next_object_id)) {
    return ParseError::NoMapNextObjectId;
  }

  if (auto tile_format = node["tile-format"]) {
    if (const auto err = parse_tile_format(tile_format, data.tile_format);
        err != ParseError::None) {
      return err;
    }
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_component_definitions(node, data); err != ParseError::None) {
    return err;
  }

  if (auto seq = node["tilesets"]) {
    if (const auto err = parse_tilesets(seq, data, dir); err != ParseError::None) {
      return err;
    }
  }

  if (auto seq = node["layers"]) {
    if (const auto err = parse_layers(seq, data); err != ParseError::None) {
      return err;
    }
  }

  if (const auto err = parse_properties(node, data.context); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_components(node, data, data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_yaml_map(const fs::path& path) -> ParseResult
{
  ParseResult result;
  result.set_path(path);

  const auto error = parse_map(path, result.data());
  result.set_error(error);

  return result;
}

}  // namespace tactile::io