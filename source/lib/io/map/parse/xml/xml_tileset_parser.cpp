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

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_fancy_tiles(XMLNode tileset_node)
    -> Expected<ir::TilesetData::MetaTiles, ParseError>
{
  ir::TilesetData::MetaTiles tiles;

  for (const auto tile_node: tileset_node.children("tile")) {
    TileID tile_id {};

    if (const auto id = as_int(tile_node, "id")) {
      tile_id = *id;
    }
    else {
      return error(ParseError::NoFancyTileId);
    }

    auto& tile = tiles[tile_id];

    for (const auto frame_node: tile_node.child("animation").children("frame")) {
      auto& frame = tile.frames.emplace_back();

      if (const auto index = as_int(frame_node, "tileid")) {
        frame.tile_index = *index;
      }
      else {
        return error(ParseError::NoAnimationFrameTile);
      }

      if (const auto duration = as_uint(frame_node, "duration")) {
        frame.duration_ms = *duration;
      }
      else {
        return error(ParseError::NoAnimationFrameDuration);
      }
    }

    for (const auto object_node: tile_node.child("objectgroup").children("object")) {
      if (auto object = parse_object(object_node)) {
        tile.objects.push_back(std::move(*object));
      }
      else {
        return pass_on_error(object);
      }
    }

    if (auto props = parse_properties(tile_node)) {
      tile.context.properties = std::move(*props);
    }
    else {
      return pass_on_error(props);
    }
  }

  return tiles;
}

[[nodiscard]] auto parse_image_info(XMLNode tileset_node,
                                    ir::TilesetData& tileset,
                                    const Path& dir) -> ParseError
{
  const auto image_node = tileset_node.child("image");

  if (const auto width = as_int(image_node, "width")) {
    tileset.image_size.x = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = as_int(image_node, "height")) {
    tileset.image_size.y = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  const auto relative_path = as_string(image_node, "source");
  if (!relative_path) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolute_path = fs::weakly_canonical(dir / *relative_path);
  if (fs::exists(absolute_path)) {
    tileset.image_path = std::move(absolute_path);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_common_attributes(XMLNode node,
                                           const TileID first_id,
                                           const Path& dir)
    -> Expected<ir::TilesetData, ParseError>
{
  ir::TilesetData tileset;
  tileset.first_tile = first_id;

  if (auto name = as_string(node, "name")) {
    tileset.name = std::move(*name);
  }
  else {
    return error(ParseError::NoTilesetName);
  }

  if (const auto tw = as_int(node, "tilewidth")) {
    tileset.tile_size.x = *tw;
  }
  else {
    return error(ParseError::NoTilesetTileWidth);
  }

  if (const auto th = as_int(node, "tileheight")) {
    tileset.tile_size.y = *th;
  }
  else {
    return error(ParseError::NoTilesetTileHeight);
  }

  if (const auto count = as_int(node, "tilecount")) {
    tileset.tile_count = *count;
  }
  else {
    return error(ParseError::NoTilesetTileCount);
  }

  if (const auto columns = as_int(node, "columns")) {
    tileset.column_count = *columns;
  }
  else {
    return error(ParseError::NoTilesetColumnCount);
  }

  if (const auto ec = parse_image_info(node, tileset, dir); ec != ParseError::None) {
    return error(ec);
  }

  if (auto tiles = parse_fancy_tiles(node)) {
    tileset.fancy_tiles = std::move(*tiles);
  }
  else {
    return pass_on_error(tiles);
  }

  if (auto props = parse_properties(node)) {
    tileset.context.properties = std::move(*props);
  }
  else {
    return pass_on_error(props);
  }

  return tileset;
}

[[nodiscard]] auto parse_external_tileset(XMLNode node,
                                          const TileID first_id,
                                          const Path& dir)
    -> Expected<ir::TilesetData, ParseError>
{
  TACTILE_ASSERT(has_attr(node, "source"));

  const auto relative_path = as_string(node, "source").value();
  const auto source_path = fs::weakly_canonical(dir / relative_path);

  if (!fs::exists(source_path)) {
    return error(ParseError::ExternalTilesetDoesNotExist);
  }

  pugi::xml_document document;
  if (!document.load_file(source_path.c_str(),
                          pugi::parse_default | pugi::parse_trim_pcdata)) {
    return error(ParseError::UnknownExternalTilesetError);
  }

  return parse_common_attributes(document.child("tileset"), first_id, dir);
}

}  // namespace

auto parse_tileset(XMLNode node, const Path& dir) -> Expected<ir::TilesetData, ParseError>
{
  TileID first_id {};
  if (const auto id = as_int(node, "firstgid")) {
    first_id = *id;
  }
  else {
    return error(ParseError::NoTilesetFirstTileId);
  }

  if (has_attr(node, "source")) {
    return parse_external_tileset(node, first_id, dir);
  }
  else {
    return parse_common_attributes(node, first_id, dir);
  }
}

}  // namespace tactile::io