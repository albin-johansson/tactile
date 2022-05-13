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

#include "xml_tileset_parser.hpp"

#include <filesystem>  // weakly_canonical, exists
#include <utility>     // move

#include "io/maps/ir.hpp"
#include "io/maps/parser/xml/xml_attribute_parser.hpp"
#include "io/maps/parser/xml/xml_layer_parser.hpp"
#include "io/maps/xml_utils.hpp"
#include "misc/assert.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_fancy_tiles(pugi::xml_node tilesetNode,
                                      ir::TilesetData& tilesetData) -> ParseError
{
  for (auto tileNode : tilesetNode.children("tile")) {
    TileID tileId{};

    if (const auto id = int_attribute(tileNode, "id")) {
      tileId = *id;
    }
    else {
      return ParseError::NoFancyTileId;
    }

    auto& tileData = tilesetData.fancy_tiles[tileId];

    for (auto frameNode : tileNode.child("animation").children("frame")) {
      auto& frameData = tileData.frames.emplace_back();

      if (const auto localId = int_attribute(frameNode, "tileid")) {
        frameData.local_id = *localId;
      }
      else {
        return ParseError::NoAnimationFrameTile;
      }

      if (const auto duration = uint_attribute(frameNode, "duration")) {
        frameData.duration_ms = *duration;
      }
      else {
        return ParseError::NoAnimationFrameDuration;
      }
    }

    for (auto objectNode : tileNode.child("objectgroup").children("object")) {
      auto& objectData = tileData.objects.emplace_back();
      if (const auto err = parse_object(objectNode, objectData);
          err != ParseError::None) {
        return err;
      }
    }

    if (const auto err = parse_properties(tileNode, tileData.context);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_image_info(pugi::xml_node tilesetNode,
                                     ir::TilesetData& tilesetData,
                                     const std::filesystem::path& dir) -> ParseError
{
  auto imageNode = tilesetNode.child("image");

  if (const auto width = int_attribute(imageNode, "width")) {
    tilesetData.image_width = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = int_attribute(imageNode, "height")) {
    tilesetData.image_height = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  const auto relativePath = string_attribute(imageNode, "source");
  if (!relativePath) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolutePath = std::filesystem::weakly_canonical(dir / *relativePath);
  if (std::filesystem::exists(absolutePath)) {
    tilesetData.image_path = std::move(absolutePath);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_common_attributes(pugi::xml_node node,
                                            ir::TilesetData& tilesetData,
                                            const std::filesystem::path& dir)
    -> ParseError
{
  if (auto name = string_attribute(node, "name")) {
    tilesetData.name = std::move(*name);
  }
  else {
    return ParseError::NoTilesetName;
  }

  if (const auto tw = int_attribute(node, "tilewidth")) {
    tilesetData.tile_width = *tw;
  }
  else {
    return ParseError::NoTilesetTileWidth;
  }

  if (const auto th = int_attribute(node, "tileheight")) {
    tilesetData.tile_height = *th;
  }
  else {
    return ParseError::NoTilesetTileHeight;
  }

  if (const auto count = int_attribute(node, "tilecount")) {
    tilesetData.tile_count = *count;
  }
  else {
    return ParseError::NoTilesetTileCount;
  }

  if (const auto columns = int_attribute(node, "columns")) {
    tilesetData.column_count = *columns;
  }
  else {
    return ParseError::NoTilesetColumnCount;
  }

  if (const auto err = _parse_image_info(node, tilesetData, dir);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = _parse_fancy_tiles(node, tilesetData); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(node, tilesetData.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_external_tileset(pugi::xml_node node,
                                           ir::TilesetData& tilesetData,
                                           const std::filesystem::path& dir) -> ParseError
{
  TACTILE_ASSERT(has_attribute(node, "source"));

  const auto relativePath = string_attribute(node, "source").value();
  const auto sourcePath = std::filesystem::weakly_canonical(dir / relativePath);

  if (!std::filesystem::exists(sourcePath)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  pugi::xml_document document;
  if (!document.load_file(sourcePath.c_str())) {
    return ParseError::UnknownExternalTilesetError;
  }

  return _parse_common_attributes(document.child("tileset"), tilesetData, dir);
}

}  // namespace

auto parse_tileset(pugi::xml_node node,
                   ir::TilesetData& tilesetData,
                   const std::filesystem::path& dir) -> ParseError
{
  if (const auto firstTile = int_attribute(node, "firstgid")) {
    tilesetData.first_tile = *firstTile;
  }
  else {
    return ParseError::NoTilesetFirstTileId;
  }

  if (has_attribute(node, "source")) {
    return _parse_external_tileset(node, tilesetData, dir);
  }
  else {
    return _parse_common_attributes(node, tilesetData, dir);
  }
}

}  // namespace tactile::parsing