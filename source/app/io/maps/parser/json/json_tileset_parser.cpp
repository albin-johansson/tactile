#include "json_tileset_parser.hpp"

#include <filesystem>  // weakly_canonical, exists
#include <string>      // string
#include <utility>     // move

#include <nlohmann/json.hpp>

#include "assert.hpp"
#include "io/maps/json_utils.hpp"
#include "json_attribute_parser.hpp"
#include "json_layer_parser.hpp"
#include "logging.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_fancy_tile(const nlohmann::json& json,
                                     ir::tileset_data& tilesetData) -> parse_error
{
  tile_id tileId{};

  if (const auto id = as_int(json, "id")) {
    tileId = *id;
  }
  else {
    return parse_error::no_fancy_tile_id;
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
        return parse_error::no_animation_frame_tile;
      }

      if (const auto duration = as_uint(frameJson, "duration")) {
        frameData.duration_ms = *duration;
      }
      else {
        return parse_error::no_animation_frame_duration;
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
            err != parse_error::none) {
          return err;
        }
      }
    }
  }

  if (const auto err = parse_properties(json, tileData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_fancy_tiles(const nlohmann::json& json,
                                      ir::tileset_data& tilesetData) -> parse_error
{
  if (json.contains("tiles")) {
    for (const auto& [_, value] : json.at("tiles").items()) {
      if (const auto err = _parse_fancy_tile(value, tilesetData);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_image_path(const nlohmann::json& json,
                                     ir::tileset_data& tilesetData,
                                     const std::filesystem::path& dir) -> parse_error
{
  const auto relative = json.find("image");

  if (relative == json.end()) {
    return parse_error::no_tileset_image_path;
  }

  auto absolute = std::filesystem::weakly_canonical(dir / relative->get<std::string>());
  if (std::filesystem::exists(absolute)) {
    tilesetData.image_path = std::move(absolute);
  }
  else {
    return parse_error::tileset_image_does_not_exist;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_common_tileset_attributes(const nlohmann::json& json,
                                                    ir::tileset_data& tilesetData,
                                                    const std::filesystem::path& dir)
    -> parse_error
{
  if (auto name = as_string(json, "name")) {
    tilesetData.name = std::move(*name);
  }
  else {
    return parse_error::no_tileset_name;
  }

  if (const auto tw = as_int(json, "tilewidth")) {
    tilesetData.tile_width = *tw;
  }
  else {
    return parse_error::no_tileset_tile_width;
  }

  if (const auto th = as_int(json, "tileheight")) {
    tilesetData.tile_height = *th;
  }
  else {
    return parse_error::no_tileset_tile_height;
  }

  if (const auto count = as_int(json, "tilecount")) {
    tilesetData.tile_count = *count;
  }
  else {
    return parse_error::no_tileset_tile_count;
  }

  if (const auto columns = as_int(json, "columns")) {
    tilesetData.column_count = *columns;
  }
  else {
    return parse_error::no_tileset_column_count;
  }

  if (const auto width = as_int(json, "imagewidth")) {
    tilesetData.image_width = *width;
  }
  else {
    return parse_error::no_tileset_image_width;
  }

  if (const auto height = as_int(json, "imageheight")) {
    tilesetData.image_height = *height;
  }
  else {
    return parse_error::no_tileset_image_height;
  }

  if (const auto err = _parse_image_path(json, tilesetData, dir);
      err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_properties(json, tilesetData.context);
      err != parse_error::none) {
    return err;
  }

  if (const auto err = _parse_fancy_tiles(json, tilesetData); err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_external_tileset(const nlohmann::json& json,
                                           ir::tileset_data& tilesetData,
                                           const std::filesystem::path& dir)
    -> parse_error
{
  TACTILE_ASSERT(json.contains("source"));

  const auto relative = json.at("source").get<std::string>();
  const auto source = std::filesystem::weakly_canonical(dir / relative);

  if (!std::filesystem::exists(source)) {
    return parse_error::external_tileset_does_not_exist;
  }

  if (const auto external = read_json(source)) {
    return _parse_common_tileset_attributes(*external, tilesetData, dir);
  }
  else {
    return parse_error::unknown_external_tileset_error;
  }
}

[[nodiscard]] auto _parse_tileset(const nlohmann::json& json,
                                  ir::tileset_data& tilesetData,
                                  const std::filesystem::path& dir) -> parse_error
{
  if (const auto firstTile = as_int(json, "firstgid")) {
    tilesetData.first_tile = *firstTile;
  }
  else {
    return parse_error::no_tileset_first_tile_id;
  }

  if (json.contains("source")) {
    return _parse_external_tileset(json, tilesetData, dir);
  }
  else {
    return _parse_common_tileset_attributes(json, tilesetData, dir);
  }

  return parse_error::none;
}

}  // namespace

auto parse_tilesets(const nlohmann::json& json,
                    ir::map_data& mapData,
                    const std::filesystem::path& dir) -> parse_error
{
  const auto iter = json.find("tilesets");

  if (iter == json.end()) {
    log_warning("JSON map has no \"tilesets\" attribute, which is required!");
    return parse_error::none;
  }

  mapData.tilesets.reserve(iter->size());

  for (const auto& [_, value] : iter->items()) {
    auto& tilesetData = mapData.tilesets.emplace_back();
    if (const auto err = _parse_tileset(value, tilesetData, dir);
        err != parse_error::none) {
      return err;
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing