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

#include "io/maps/ir.hpp"
#include "io/maps/parser/yaml/yaml_attribute_parser.hpp"
#include "io/maps/parser/yaml/yaml_layer_parser.hpp"
#include "tactile.hpp"

namespace tactile::parsing {
namespace {

constexpr int32 _tileset_file_version = 1;

[[nodiscard]] auto _parse_animation_frame(const YAML::Node& node, ir::MetaTileData& tile)
    -> ParseError
{
  TileID frameTile{};
  uint64 frameDuration{};

  if (auto id = node["tile"]) {
    frameTile = id.as<TileID>();
  }
  else {
    return ParseError::NoAnimationFrameTile;
  }

  if (auto duration = node["duration"]) {
    frameDuration = duration.as<uint64>();
  }
  else {
    return ParseError::NoAnimationFrameDuration;
  }

  auto& frame = tile.frames.emplace_back();
  frame.local_id = frameTile;
  frame.duration_ms = frameDuration;

  return ParseError::None;
}

[[nodiscard]] auto _parse_fancy_tile(const YAML::Node& node,
                                     const ir::MapData& map,
                                     ir::TilesetData& tileset) -> ParseError
{
  TileID tileId{};

  if (auto id = node["id"]) {
    tileId = id.as<TileID>();
  }
  else {
    return ParseError::NoFancyTileId;
  }

  auto& tile = tileset.fancy_tiles[tileId];

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
      if (const auto err = parse_object(objectNode, map, &object);
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

    if (auto version = node["version"]) {
      if (version.as<int32>() != _tileset_file_version) {
        return ParseError::UnsupportedTilesetVersion;
      }
    }
    else {
      return ParseError::NoTilesetVersion;
    }

    if (auto name = node["name"]) {
      tileset.name = name.as<std::string>();
    }
    else {
      return ParseError::NoTilesetName;
    }

    if (auto count = node["tile-count"]) {
      tileset.tile_count = count.as<int32>();
    }
    else {
      return ParseError::NoTilesetTileCount;
    }

    if (auto tw = node["tile-width"]) {
      tileset.tile_width = tw.as<int32>();
    }
    else {
      return ParseError::NoTilesetTileWidth;
    }

    if (auto th = node["tile-height"]) {
      tileset.tile_height = th.as<int32>();
    }
    else {
      return ParseError::NoTilesetTileHeight;
    }

    if (auto columns = node["column-count"]) {
      tileset.column_count = columns.as<int32>();
    }
    else {
      return ParseError::NoTilesetColumnCount;
    }

    if (auto rel = node["image-path"]) {
      auto abs = std::filesystem::weakly_canonical(dir / rel.as<std::string>());
      if (std::filesystem::exists(abs)) {
        tileset.image_path = std::move(abs);
      }
      else {
        return ParseError::TilesetImageDoesNotExist;
      }
    }
    else {
      return ParseError::NoTilesetImagePath;
    }

    if (auto imageWidth = node["image-width"]) {
      tileset.image_width = imageWidth.as<int32>();
    }
    else {
      return ParseError::NoTilesetImageWidth;
    }

    if (auto imageHeight = node["image-height"]) {
      tileset.image_height = imageHeight.as<int32>();
    }
    else {
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
    auto first = node["first-global-id"];
    auto path = node["path"];

    if (!first) {
      return ParseError::NoTilesetFirstTileId;
    }

    if (!path) {
      return ParseError::NoExternalTilesetPath;
    }

    const auto source = std::filesystem::weakly_canonical(dir / path.as<std::string>());

    if (std::filesystem::exists(source)) {
      if (const auto err = _parse_tileset(source, map, first.as<TileID>());
          err != ParseError::None) {
        return err;
      }
    }
    else {
      return ParseError::ExternalTilesetDoesNotExist;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::parsing
