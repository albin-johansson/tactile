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

#include <utility>  // move

#include "core/common/filesystem.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_attribute_parser.hpp"
#include "io/map/parse/xml/xml_layer_parser.hpp"
#include "io/util/xml.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto _parse_fancy_tiles(XMLNode tilesetNode, ir::TilesetData& tilesetData)
    -> ParseError
{
  for (auto tileNode : tilesetNode.children("tile")) {
    TileID tileId {};

    if (const auto id = as_int(tileNode, "id")) {
      tileId = *id;
    }
    else {
      return ParseError::NoFancyTileId;
    }

    auto& tileData = tilesetData.fancy_tiles[tileId];

    for (auto frameNode : tileNode.child("animation").children("frame")) {
      auto& frameData = tileData.frames.emplace_back();

      if (const auto localId = as_int(frameNode, "tileid")) {
        frameData.local_id = *localId;
      }
      else {
        return ParseError::NoAnimationFrameTile;
      }

      if (const auto duration = as_uint(frameNode, "duration")) {
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

[[nodiscard]] auto _parse_image_info(XMLNode          tilesetNode,
                                     ir::TilesetData& tilesetData,
                                     const fs::path&  dir) -> ParseError
{
  auto imageNode = tilesetNode.child("image");

  if (const auto width = as_int(imageNode, "width")) {
    tilesetData.image_size.x = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = as_int(imageNode, "height")) {
    tilesetData.image_size.y = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  const auto relativePath = as_string(imageNode, "source");
  if (!relativePath) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolutePath = fs::weakly_canonical(dir / *relativePath);
  if (fs::exists(absolutePath)) {
    tilesetData.image_path = std::move(absolutePath);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_common_attributes(XMLNode          node,
                                            ir::TilesetData& tilesetData,
                                            const fs::path&  dir) -> ParseError
{
  if (auto name = as_string(node, "name")) {
    tilesetData.name = std::move(*name);
  }
  else {
    return ParseError::NoTilesetName;
  }

  if (const auto tw = as_int(node, "tilewidth")) {
    tilesetData.tile_size.x = *tw;
  }
  else {
    return ParseError::NoTilesetTileWidth;
  }

  if (const auto th = as_int(node, "tileheight")) {
    tilesetData.tile_size.y = *th;
  }
  else {
    return ParseError::NoTilesetTileHeight;
  }

  if (const auto count = as_int(node, "tilecount")) {
    tilesetData.tile_count = *count;
  }
  else {
    return ParseError::NoTilesetTileCount;
  }

  if (const auto columns = as_int(node, "columns")) {
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

[[nodiscard]] auto _parse_external_tileset(XMLNode          node,
                                           ir::TilesetData& tilesetData,
                                           const fs::path&  dir) -> ParseError
{
  TACTILE_ASSERT(has_attr(node, "source"));

  const auto relativePath = as_string(node, "source").value();
  const auto sourcePath = fs::weakly_canonical(dir / relativePath);

  if (!fs::exists(sourcePath)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  pugi::xml_document document;
  if (!document.load_file(sourcePath.c_str())) {
    return ParseError::UnknownExternalTilesetError;
  }

  return _parse_common_attributes(document.child("tileset"), tilesetData, dir);
}

}  // namespace

auto parse_tileset(XMLNode node, ir::TilesetData& tilesetData, const fs::path& dir)
    -> ParseError
{
  if (const auto firstTile = as_int(node, "firstgid")) {
    tilesetData.first_tile = *firstTile;
  }
  else {
    return ParseError::NoTilesetFirstTileId;
  }

  if (has_attr(node, "source")) {
    return _parse_external_tileset(node, tilesetData, dir);
  }
  else {
    return _parse_common_attributes(node, tilesetData, dir);
  }
}

}  // namespace tactile::io