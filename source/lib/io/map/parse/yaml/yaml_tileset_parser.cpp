/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <utility>  // move

#include "common/primitives.hpp"
#include "common/type/string.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/yaml.hpp"

namespace tactile {
namespace {

constexpr int32 kTilesetFormatVersion = 1;

[[nodiscard]] auto _parse_animation_frame(const YAML::Node& node)
    -> Parsed<AnimationFrameIR>
{
  TileIndex tile_index {};
  if (!read_attr(node, "tile", tile_index)) {
    return unexpected(ParseError::NoAnimationFrameTile);
  }

  uint64 duration {};
  if (!read_attr(node, "duration", duration)) {
    return unexpected(ParseError::NoAnimationFrameDuration);
  }

  AnimationFrameIR frame;
  frame.tile_index = tile_index;
  frame.duration_ms = duration;

  return frame;
}

[[nodiscard]] auto _parse_fancy_tile(const YAML::Node& node, const MapIR& map)
    -> Parsed<TileIR>
{
  TileIR tile;

  if (auto sequence = node["animation"]) {
    tile.frames.reserve(sequence.size());

    for (const auto& frame_node: sequence) {
      if (auto frame = _parse_animation_frame(frame_node)) {
        tile.frames.push_back(*frame);
      }
      else {
        return propagate_unexpected(frame);
      }
    }
  }

  if (auto sequence = node["objects"]) {
    tile.objects.reserve(sequence.size());

    for (const auto& object_node: sequence) {
      if (auto object = parse_object(object_node, map)) {
        tile.objects.push_back(std::move(*object));
      }
      else {
        return propagate_unexpected(object);
      }
    }
  }

  if (auto context = parse_context(node, map)) {
    tile.context = std::move(*context);
  }
  else {
    return propagate_unexpected(context);
  }

  return tile;
}

[[nodiscard]] auto _parse_fancy_tiles(const YAML::Node& sequence, const MapIR& map)
    -> Parsed<TilesetIR::MetaTiles>
{
  TilesetIR::MetaTiles fancy_tiles;
  fancy_tiles.reserve(sequence.size());

  for (const auto& node: sequence) {
    TileID id {};
    if (!read_attr(node, "id", id)) {
      return unexpected(ParseError::NoFancyTileId);
    }

    if (auto tile = _parse_fancy_tile(node, map)) {
      fancy_tiles[id] = std::move(*tile);
    }
    else {
      return propagate_unexpected(tile);
    }
  }

  return fancy_tiles;
}

[[nodiscard]] auto _parse_tileset(const MapIR& map,
                                  const Path& source,
                                  const TileID first_tile_id) -> Parsed<TilesetIR>
{
  try {
    const auto node = YAML::LoadFile(source.string());
    if (!node) {
      return unexpected(ParseError::UnknownExternalTilesetError);
    }

    const auto dir = source.parent_path();

    TilesetIR tileset;
    tileset.first_tile = first_tile_id;

    int32 version {};
    if (!read_attr(node, "version", version)) {
      return unexpected(ParseError::NoTilesetVersion);
    }

    if (version != kTilesetFormatVersion) {
      return unexpected(ParseError::UnsupportedTilesetVersion);
    }

    if (!read_attr(node, "name", tileset.name)) {
      return unexpected(ParseError::NoTilesetName);
    }

    if (!read_attr(node, "tile-count", tileset.tile_count)) {
      return unexpected(ParseError::NoTilesetTileCount);
    }

    if (!read_attr(node, "tile-width", tileset.tile_size.x)) {
      return unexpected(ParseError::NoTilesetTileWidth);
    }

    if (!read_attr(node, "tile-height", tileset.tile_size.y)) {
      return unexpected(ParseError::NoTilesetTileHeight);
    }

    if (!read_attr(node, "column-count", tileset.column_count)) {
      return unexpected(ParseError::NoTilesetColumnCount);
    }

    String relative;
    if (!read_attr(node, "image-path", relative)) {
      return unexpected(ParseError::NoTilesetImagePath);
    }

    auto absolute = fs::weakly_canonical(dir / relative);
    if (fs::exists(absolute)) {
      tileset.image_path = std::move(absolute);
    }
    else {
      return unexpected(ParseError::TilesetImageDoesNotExist);
    }

    if (!read_attr(node, "image-width", tileset.image_size.x)) {
      return unexpected(ParseError::NoTilesetImageWidth);
    }

    if (!read_attr(node, "image-height", tileset.image_size.y)) {
      return unexpected(ParseError::NoTilesetImageHeight);
    }

    if (auto sequence = node["tiles"]) {
      if (auto meta_tiles = _parse_fancy_tiles(sequence, map)) {
        tileset.fancy_tiles = std::move(*meta_tiles);
      }
      else {
        return propagate_unexpected(meta_tiles);
      }
    }

    if (auto context = parse_context(node, map)) {
      tileset.context = std::move(*context);
    }
    else {
      return propagate_unexpected(context);
    }

    return tileset;
  }
  catch (...) {
    return unexpected(ParseError::UnknownExternalTilesetError);
  }
}

}  // namespace

auto parse_tilesets(const YAML::Node& sequence, const MapIR& map, const Path& dir)
    -> Parsed<Vector<TilesetIR>>
{
  Vector<TilesetIR> tilesets;
  tilesets.reserve(sequence.size());

  for (const auto& node: sequence) {
    TileID first {};
    if (!read_attr(node, "first-global-id", first)) {
      return unexpected(ParseError::NoTilesetFirstTileId);
    }

    String path;
    if (!read_attr(node, "path", path)) {
      return unexpected(ParseError::NoExternalTilesetPath);
    }

    const auto source = fs::weakly_canonical(dir / path);

    if (fs::exists(source)) {
      if (auto tileset = _parse_tileset(map, source, first)) {
        tilesets.push_back(std::move(*tileset));
      }
      else {
        return propagate_unexpected(tileset);
      }
    }
    else {
      return unexpected(ParseError::ExternalTilesetDoesNotExist);
    }
  }

  return tilesets;
}

}  // namespace tactile
