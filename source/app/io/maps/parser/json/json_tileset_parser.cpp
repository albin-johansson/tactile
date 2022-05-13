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

#include "json_tileset_parser.hpp"

#include <filesystem>  // weakly_canonical, exists
#include <string>      // string
#include <utility>     // move

#include <nlohmann/json.hpp>

#include "io/maps/json_utils.hpp"
#include "json_attribute_parser.hpp"
#include "json_layer_parser.hpp"
#include "misc/assert.hpp"
#include "misc/logging.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_fancy_tile(const nlohmann::json& json,
                                     ir::TilesetData& tilesetData) -> ParseError
{
  TileID tileId{};

  if (const auto id = as_int(json, "id")) {
    tileId = *id;
  }
  else {
    return ParseError::NoFancyTileId;
  }

  auto& tileData = tilesetData.fancy_tiles[tileId];

  if (const auto iter = json.find("animation"); iter != json.end()) {
    tileData.frames.reserve(iter->size());

    for (const auto& [_, frameJson] : iter->items()) {
      auto& frameData = tileData.frames.emplace_back();

      if (const auto localId = as_int(frameJson, "tileid")) {
        frameData.local_id = *localId;
      }
      else {
        return ParseError::NoAnimationFrameTile;
      }

      if (const auto duration = as_uint(frameJson, "duration")) {
        frameData.duration_ms = *duration;
      }
      else {
        return ParseError::NoAnimationFrameDuration;
      }
    }
  }

  if (const auto layerIter = json.find("objectgroup"); layerIter != json.end()) {
    if (const auto objectsIter = layerIter->find("objects");
        objectsIter != layerIter->end()) {
      tileData.objects.reserve(objectsIter->size());
      for (const auto& [_, objectJson] : objectsIter->items()) {
        auto& objectData = tileData.objects.emplace_back();
        if (const auto err = parse_object(objectJson, objectData);
            err != ParseError::None) {
          return err;
        }
      }
    }
  }

  if (const auto err = parse_properties(json, tileData.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_fancy_tiles(const nlohmann::json& json,
                                      ir::TilesetData& tilesetData) -> ParseError
{
  if (json.contains("tiles")) {
    for (const auto& [_, value] : json.at("tiles").items()) {
      if (const auto err = _parse_fancy_tile(value, tilesetData);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_image_path(const nlohmann::json& json,
                                     ir::TilesetData& tilesetData,
                                     const std::filesystem::path& dir) -> ParseError
{
  const auto relative = json.find("image");

  if (relative == json.end()) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolute = std::filesystem::weakly_canonical(dir / relative->get<std::string>());
  if (std::filesystem::exists(absolute)) {
    tilesetData.image_path = std::move(absolute);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_common_tileset_attributes(const nlohmann::json& json,
                                                    ir::TilesetData& tilesetData,
                                                    const std::filesystem::path& dir)
    -> ParseError
{
  if (auto name = as_string(json, "name")) {
    tilesetData.name = std::move(*name);
  }
  else {
    return ParseError::NoTilesetName;
  }

  if (const auto tw = as_int(json, "tilewidth")) {
    tilesetData.tile_width = *tw;
  }
  else {
    return ParseError::NoTilesetTileWidth;
  }

  if (const auto th = as_int(json, "tileheight")) {
    tilesetData.tile_height = *th;
  }
  else {
    return ParseError::NoTilesetTileHeight;
  }

  if (const auto count = as_int(json, "tilecount")) {
    tilesetData.tile_count = *count;
  }
  else {
    return ParseError::NoTilesetTileCount;
  }

  if (const auto columns = as_int(json, "columns")) {
    tilesetData.column_count = *columns;
  }
  else {
    return ParseError::NoTilesetColumnCount;
  }

  if (const auto width = as_int(json, "imagewidth")) {
    tilesetData.image_width = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = as_int(json, "imageheight")) {
    tilesetData.image_height = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  if (const auto err = _parse_image_path(json, tilesetData, dir);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(json, tilesetData.context);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = _parse_fancy_tiles(json, tilesetData); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_external_tileset(const nlohmann::json& json,
                                           ir::TilesetData& tilesetData,
                                           const std::filesystem::path& dir) -> ParseError
{
  TACTILE_ASSERT(json.contains("source"));

  const auto relative = json.at("source").get<std::string>();
  const auto source = std::filesystem::weakly_canonical(dir / relative);

  if (!std::filesystem::exists(source)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  if (const auto external = read_json(source)) {
    return _parse_common_tileset_attributes(*external, tilesetData, dir);
  }
  else {
    return ParseError::UnknownExternalTilesetError;
  }
}

[[nodiscard]] auto _parse_tileset(const nlohmann::json& json,
                                  ir::TilesetData& tilesetData,
                                  const std::filesystem::path& dir) -> ParseError
{
  if (const auto firstTile = as_int(json, "firstgid")) {
    tilesetData.first_tile = *firstTile;
  }
  else {
    return ParseError::NoTilesetFirstTileId;
  }

  if (json.contains("source")) {
    return _parse_external_tileset(json, tilesetData, dir);
  }
  else {
    return _parse_common_tileset_attributes(json, tilesetData, dir);
  }
}

}  // namespace

auto parse_tilesets(const nlohmann::json& json,
                    ir::MapData& mapData,
                    const std::filesystem::path& dir) -> ParseError
{
  const auto iter = json.find("tilesets");

  if (iter == json.end()) {
    log_warning("JSON map has no \"tilesets\" attribute, which is required!");
    return ParseError::None;
  }

  mapData.tilesets.reserve(iter->size());

  for (const auto& [_, value] : iter->items()) {
    auto& tilesetData = mapData.tilesets.emplace_back();
    if (const auto err = _parse_tileset(value, tilesetData, dir);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::parsing