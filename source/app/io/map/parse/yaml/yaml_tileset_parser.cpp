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

#include <string>   // string
#include <utility>  // move

#include "core/common/fs.hpp"
#include "core/common/ints.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/yaml.hpp"

namespace tactile::io {
namespace {

constexpr int32 tileset_format_version = 1;

[[nodiscard]] auto parse_animation_frame(const YAML::Node& node)
    -> Expected<ir::AnimationFrameData, ParseError>
{
  TileIndex tile_index {};
  if (!read_attribute(node, "tile", tile_index)) {
    return error(ParseError::NoAnimationFrameTile);
  }

  uint64 duration {};
  if (!read_attribute(node, "duration", duration)) {
    return error(ParseError::NoAnimationFrameDuration);
  }

  ir::AnimationFrameData frame;
  frame.tile_index = tile_index;
  frame.duration_ms = duration;

  return frame;
}

[[nodiscard]] auto parse_fancy_tile(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::MetaTileData, ParseError>
{
  ir::MetaTileData tile;

  if (auto sequence = node["animation"]) {
    tile.frames.reserve(sequence.size());

    for (const auto& frame_node : sequence) {
      if (auto frame = parse_animation_frame(frame_node)) {
        tile.frames.push_back(*frame);
      }
      else {
        return pass_on_error(frame);
      }
    }
  }

  if (auto sequence = node["objects"]) {
    tile.objects.reserve(sequence.size());

    for (const auto& object_node : sequence) {
      if (auto object = parse_object(object_node, map)) {
        tile.objects.push_back(std::move(*object));
      }
      else {
        return pass_on_error(object);
      }
    }
  }

  if (auto context = parse_context(node, map)) {
    tile.context = std::move(*context);
  }
  else {
    return pass_on_error(context);
  }

  return tile;
}

[[nodiscard]] auto parse_fancy_tiles(const YAML::Node& sequence, const ir::MapData& map)
    -> Expected<ir::TilesetData::MetaTiles, ParseError>
{
  ir::TilesetData::MetaTiles fancy_tiles;
  fancy_tiles.reserve(sequence.size());

  for (const auto& node : sequence) {
    TileID id {};
    if (!read_attribute(node, "id", id)) {
      return error(ParseError::NoFancyTileId);
    }

    if (auto tile = parse_fancy_tile(node, map)) {
      fancy_tiles[id] = std::move(*tile);
    }
    else {
      return pass_on_error(tile);
    }
  }

  return fancy_tiles;
}

[[nodiscard]] auto parse_tileset(const ir::MapData& map,
                                 const fs::path& source,
                                 const TileID first_tile_id)
    -> Expected<ir::TilesetData, ParseError>
{
  try {
    const auto node = YAML::LoadFile(source.string());
    if (!node) {
      return error(ParseError::UnknownExternalTilesetError);
    }

    const auto dir = source.parent_path();

    ir::TilesetData tileset;
    tileset.first_tile = first_tile_id;

    int32 version {};
    if (!read_attribute(node, "version", version)) {
      return error(ParseError::NoTilesetVersion);
    }

    if (version != tileset_format_version) {
      return error(ParseError::UnsupportedTilesetVersion);
    }

    if (!read_attribute(node, "name", tileset.name)) {
      return error(ParseError::NoTilesetName);
    }

    if (!read_attribute(node, "tile-count", tileset.tile_count)) {
      return error(ParseError::NoTilesetTileCount);
    }

    if (!read_attribute(node, "tile-width", tileset.tile_size.x)) {
      return error(ParseError::NoTilesetTileWidth);
    }

    if (!read_attribute(node, "tile-height", tileset.tile_size.y)) {
      return error(ParseError::NoTilesetTileHeight);
    }

    if (!read_attribute(node, "column-count", tileset.column_count)) {
      return error(ParseError::NoTilesetColumnCount);
    }

    std::string relative;
    if (!read_attribute(node, "image-path", relative)) {
      return error(ParseError::NoTilesetImagePath);
    }

    auto absolute = fs::weakly_canonical(dir / relative);
    if (fs::exists(absolute)) {
      tileset.image_path = std::move(absolute);
    }
    else {
      return error(ParseError::TilesetImageDoesNotExist);
    }

    if (!read_attribute(node, "image-width", tileset.image_size.x)) {
      return error(ParseError::NoTilesetImageWidth);
    }

    if (!read_attribute(node, "image-height", tileset.image_size.y)) {
      return error(ParseError::NoTilesetImageHeight);
    }

    if (auto sequence = node["tiles"]) {
      if (auto meta_tiles = parse_fancy_tiles(sequence, map)) {
        tileset.fancy_tiles = std::move(*meta_tiles);
      }
      else {
        return pass_on_error(meta_tiles);
      }
    }

    if (auto context = parse_context(node, map)) {
      tileset.context = std::move(*context);
    }
    else {
      return pass_on_error(context);
    }

    return tileset;
  }
  catch (...) {
    return error(ParseError::UnknownExternalTilesetError);
  }
}

}  // namespace

auto parse_tilesets(const YAML::Node& sequence,
                    const ir::MapData& map,
                    const fs::path& dir)
    -> Expected<std::vector<ir::TilesetData>, ParseError>
{
  std::vector<ir::TilesetData> tilesets;
  tilesets.reserve(sequence.size());

  for (const auto& node : sequence) {
    TileID first {};
    if (!read_attribute(node, "first-global-id", first)) {
      return error(ParseError::NoTilesetFirstTileId);
    }

    std::string path;
    if (!read_attribute(node, "path", path)) {
      return error(ParseError::NoExternalTilesetPath);
    }

    const auto source = fs::weakly_canonical(dir / path);

    if (fs::exists(source)) {
      if (auto tileset = parse_tileset(map, source, first)) {
        tilesets.push_back(std::move(*tileset));
      }
      else {
        return pass_on_error(tileset);
      }
    }
    else {
      return error(ParseError::ExternalTilesetDoesNotExist);
    }
  }

  return tilesets;
}

}  // namespace tactile::io
