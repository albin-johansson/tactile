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

#include "yaml_tileset_parser.hpp"

#include <filesystem>  // exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "core/common/ints.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/parser/yaml/yaml_attribute_parser.hpp"
#include "io/maps/parser/yaml/yaml_layer_parser.hpp"
#include "io/maps/yaml_utils.hpp"

namespace tactile::io {
namespace {

constexpr int32 _tileset_file_version = 1;

[[nodiscard]] auto _parse_animation_frame(const YAML::Node& node, ir::MetaTileData& tile)
    -> ParseError
{
  TileIndex tileIndex{};
  if (!read_attribute(node, "tile", tileIndex)) {
    return ParseError::NoAnimationFrameTile;
  }

  uint64 duration{};
  if (!read_attribute(node, "duration", duration)) {
    return ParseError::NoAnimationFrameDuration;
  }

  auto& frame = tile.frames.emplace_back();
  frame.local_id = tileIndex;
  frame.duration_ms = duration;

  return ParseError::None;
}

[[nodiscard]] auto _parse_fancy_tile(const YAML::Node& node,
                                     const ir::MapData& map,
                                     ir::TilesetData& tileset) -> ParseError
{
  TileID id{};
  if (!read_attribute(node, "id", id)) {
    return ParseError::NoFancyTileId;
  }

  auto& tile = tileset.fancy_tiles[id];

  if (auto sequence = node["animation"]) {
    tile.frames.reserve(sequence.size());

    for (const auto& frame : sequence) {
      if (const auto err = _parse_animation_frame(frame, tile); err != ParseError::None) {
        return err;
      }
    }
  }

  if (auto sequence = node["objects"]) {
    tile.objects.reserve(sequence.size());

    for (const auto& objectNode : sequence) {
      auto& object = tile.objects.emplace_back();
      if (const auto err = parse_object(objectNode, map, object);
          err != ParseError::None) {
        return err;
      }
    }
  }

  if (const auto err = parse_properties(node, tile.context); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_components(node, map, tile.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_fancy_tiles(const YAML::Node& sequence,
                                      const ir::MapData& map,
                                      ir::TilesetData& tileset) -> ParseError
{
  tileset.fancy_tiles.reserve(sequence.size());

  for (const auto& node : sequence) {
    if (const auto err = _parse_fancy_tile(node, map, tileset); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_tileset(const std::filesystem::path& source,
                                  ir::MapData& map,
                                  const TileID firstTileId) -> ParseError
{
  try {
    const auto node = YAML::LoadFile(source.string());
    if (!node) {
      return ParseError::UnknownExternalTilesetError;
    }

    const auto dir = source.parent_path();

    auto& tileset = map.tilesets.emplace_back();
    tileset.first_tile = firstTileId;

    int32 version{};
    if (!read_attribute(node, "version", version)) {
      return ParseError::NoTilesetVersion;
    }

    if (version != _tileset_file_version) {
      return ParseError::UnsupportedTilesetVersion;
    }

    if (!read_attribute(node, "name", tileset.name)) {
      return ParseError::NoTilesetName;
    }

    if (!read_attribute(node, "tile-count", tileset.tile_count)) {
      return ParseError::NoTilesetTileCount;
    }

    if (!read_attribute(node, "tile-width", tileset.tile_size.x)) {
      return ParseError::NoTilesetTileWidth;
    }

    if (!read_attribute(node, "tile-height", tileset.tile_size.y)) {
      return ParseError::NoTilesetTileHeight;
    }

    if (!read_attribute(node, "column-count", tileset.column_count)) {
      return ParseError::NoTilesetColumnCount;
    }

    std::string relative;
    if (!read_attribute(node, "image-path", relative)) {
      return ParseError::NoTilesetImagePath;
    }

    auto absolute = std::filesystem::weakly_canonical(dir / relative);
    if (std::filesystem::exists(absolute)) {
      tileset.image_path = std::move(absolute);
    }
    else {
      return ParseError::TilesetImageDoesNotExist;
    }

    if (!read_attribute(node, "image-width", tileset.image_size.x)) {
      return ParseError::NoTilesetImageWidth;
    }

    if (!read_attribute(node, "image-height", tileset.image_size.y)) {
      return ParseError::NoTilesetImageHeight;
    }

    if (auto sequence = node["tiles"]) {
      if (const auto err = _parse_fancy_tiles(sequence, map, tileset);
          err != ParseError::None) {
        return err;
      }
    }

    if (const auto err = parse_properties(node, tileset.context);
        err != ParseError::None) {
      return err;
    }

    if (const auto err = parse_components(node, map, tileset.context);
        err != ParseError::None) {
      return err;
    }

    return ParseError::None;
  }
  catch (...) {
    return ParseError::UnknownExternalTilesetError;
  }
}

}  // namespace

auto parse_tilesets(const YAML::Node& sequence,
                    ir::MapData& map,
                    const std::filesystem::path& dir) -> ParseError
{
  map.tilesets.reserve(sequence.size());

  for (const auto& node : sequence) {
    TileID first{};
    if (!read_attribute(node, "first-global-id", first)) {
      return ParseError::NoTilesetFirstTileId;
    }

    std::string path;
    if (!read_attribute(node, "path", path)) {
      return ParseError::NoExternalTilesetPath;
    }

    const auto source = std::filesystem::weakly_canonical(dir / path);

    if (std::filesystem::exists(source)) {
      if (const auto err = _parse_tileset(source, map, first); err != ParseError::None) {
        return err;
      }
    }
    else {
      return ParseError::ExternalTilesetDoesNotExist;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io
