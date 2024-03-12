// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <string>   // string
#include <utility>  // move

#include <spdlog/spdlog.h>

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/json/json_parser.hpp"
#include "io/util/json.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_fancy_tile(const JSON& json, TilesetIR& tileset_data)
    -> ParseError
{
  TileID tile_id {};
  if (const auto id = as_int(json, "id")) {
    tile_id = *id;
  }
  else {
    return ParseError::NoFancyTileId;
  }

  auto& tile_data = tileset_data.fancy_tiles[tile_id];

  if (const auto iter = json.find("animation"); iter != json.end()) {
    tile_data.frames.reserve(iter->size());

    for (const auto& [_, frameJson]: iter->items()) {
      auto& frame_data = tile_data.frames.emplace_back();

      if (const auto index = as_int(frameJson, "tileid")) {
        frame_data.tile_index = *index;
      }
      else {
        return ParseError::NoAnimationFrameTile;
      }

      if (const auto duration = as_uint(frameJson, "duration")) {
        frame_data.duration_ms = *duration;
      }
      else {
        return ParseError::NoAnimationFrameDuration;
      }
    }
  }

  if (const auto layer_iter = json.find("objectgroup"); layer_iter != json.end()) {
    const auto objects_iter = layer_iter->find("objects");
    if (objects_iter != layer_iter->end()) {
      tile_data.objects.reserve(objects_iter->size());

      for (const auto& [_, objectJson]: objects_iter->items()) {
        auto& object_data = tile_data.objects.emplace_back();
        if (const auto err = parse_object(objectJson, object_data);
            err != ParseError::None) {
          return err;
        }
      }
    }
  }

  if (const auto err = parse_properties(json, tile_data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_fancy_tiles(const JSON& json, TilesetIR& tileset_data)
    -> ParseError
{
  if (json.contains("tiles")) {
    for (const auto& [_, value]: json.at("tiles").items()) {
      if (const auto err = parse_fancy_tile(value, tileset_data);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_image_path(const JSON& json,
                                    TilesetIR& tileset_data,
                                    const Path& dir) -> ParseError
{
  const auto relative = json.find("image");

  if (relative == json.end()) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolute = fs::weakly_canonical(dir / relative->get<std::string>());
  if (fs::exists(absolute)) {
    tileset_data.image_path = std::move(absolute);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_common_tileset_attributes(const JSON& json,
                                                   TilesetIR& tileset_data,
                                                   const Path& dir) -> ParseError
{
  if (auto name = as_string(json, "name")) {
    tileset_data.name = std::move(*name);
  }
  else {
    return ParseError::NoTilesetName;
  }

  if (const auto tw = as_int(json, "tilewidth")) {
    tileset_data.tile_size[0] = *tw;
  }
  else {
    return ParseError::NoTilesetTileWidth;
  }

  if (const auto th = as_int(json, "tileheight")) {
    tileset_data.tile_size[1] = *th;
  }
  else {
    return ParseError::NoTilesetTileHeight;
  }

  if (const auto count = as_int(json, "tilecount")) {
    tileset_data.tile_count = *count;
  }
  else {
    return ParseError::NoTilesetTileCount;
  }

  if (const auto columns = as_int(json, "columns")) {
    tileset_data.column_count = *columns;
  }
  else {
    return ParseError::NoTilesetColumnCount;
  }

  if (const auto width = as_int(json, "imagewidth")) {
    tileset_data.image_size[0] = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = as_int(json, "imageheight")) {
    tileset_data.image_size[1] = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  if (const auto err = parse_image_path(json, tileset_data, dir);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(json, tileset_data.context);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_fancy_tiles(json, tileset_data); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_external_tileset(const JSON& json,
                                          TilesetIR& tileset_data,
                                          const Path& dir) -> ParseError
{
  TACTILE_ASSERT(json.contains("source"));

  const auto relative = json.at("source").get<std::string>();
  const auto source = fs::weakly_canonical(dir / relative);

  if (!fs::exists(source)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  if (const auto external = parse_json_file(source)) {
    return parse_common_tileset_attributes(*external, tileset_data, dir);
  }
  else {
    return ParseError::UnknownExternalTilesetError;
  }
}

[[nodiscard]] auto parse_tileset(const JSON& json,
                                 TilesetIR& tileset_data,
                                 const Path& dir) -> ParseError
{
  if (const auto first_tile = as_int(json, "firstgid")) {
    tileset_data.first_tile = *first_tile;
  }
  else {
    return ParseError::NoTilesetFirstTileId;
  }

  if (json.contains("source")) {
    return parse_external_tileset(json, tileset_data, dir);
  }
  else {
    return parse_common_tileset_attributes(json, tileset_data, dir);
  }
}

}  // namespace

auto parse_tilesets(const JSON& json, MapIR& map_data, const Path& dir) -> ParseError
{
  const auto iter = json.find("tilesets");

  if (iter == json.end()) {
    spdlog::warn("JSON map has no \"tilesets\" attribute, which is required!");
    return ParseError::None;
  }

  map_data.tilesets.reserve(iter->size());

  for (const auto& [_, value]: iter->items()) {
    auto& tileset_data = map_data.tilesets.emplace_back();
    if (const auto err = parse_tileset(value, tileset_data, dir);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace tactile
