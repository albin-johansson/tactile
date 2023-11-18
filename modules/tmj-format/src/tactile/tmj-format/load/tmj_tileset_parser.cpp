// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/load/tmj_tileset_parser.hpp"

#include <filesystem>  // exists
#include <utility>     // move

#include <fmt/std.h>

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/io/save/save_format_error.hpp"
#include "tactile/tmj-format/common/json.hpp"
#include "tactile/tmj-format/load/tmj_layer_parser.hpp"
#include "tactile/tmj-format/load/tmj_meta_parser.hpp"

namespace tactile::tmj {

auto parse_tile_animation_frame(const JSON& frame_json) -> Result<ir::AnimationFrame>
{
  ir::AnimationFrame frame {};
  return parse(frame_json, "tileid", frame.tile_index.value)  //
      .and_then([&] { return parse(frame_json, "duration", frame.duration_ms); })
      .transform([&] { return frame; });
}

auto parse_tile_animation(const JSON& json, ir::TileAnimation& animation) -> Result<void>
{
  const auto animation_iter = json.find("animation");

  if (animation_iter != json.end()) {
    animation.reserve(animation_iter->size());

    for (const auto& [_, frame_json] : animation_iter->items()) {
      if (const auto frame = parse_tile_animation_frame(frame_json)) {
        animation.push_back(*frame);
      }
      else {
        return propagate_unexpected(frame);
      }
    }
  }

  return kSuccess;
}

auto parse_tile_objects(const JSON& json, Vector<ir::Object>& objects) -> Result<void>
{
  const auto object_group_iter = json.find("objectgroup");
  if (object_group_iter == json.end()) {
    return kSuccess;
  }

  const auto objects_iter = object_group_iter->find("objects");
  if (objects_iter == object_group_iter->end()) {
    return kSuccess;
  }

  objects.reserve(objects_iter->size());

  for (const auto& [_, object_json] : objects_iter->items()) {
    if (auto object = parse_object(object_json)) {
      objects.push_back(std::move(*object));
    }
    else {
      return propagate_unexpected(object);
    }
  }

  return kSuccess;
}

auto parse_tileset_tile(const JSON& json, const ir::Tileset& tileset) -> Result<ir::Tile>
{
  ir::Tile tile {};

  return parse(json, "id", tile.index.value)
      .and_then([&] { return parse_tile_animation(json, tile.animation); })
      .and_then([&] { return parse_tile_objects(json, tile.objects); })
      .and_then([&] { return parse_metadata(json, tile.meta); })
      .transform([&] { return std::move(tile); });
}

auto parse_tileset_tiles(const JSON& json, ir::Tileset& tileset) -> Result<void>
{
  const auto tiles_iter = json.find("tiles");

  if (tiles_iter != json.end()) {
    for (const auto& [_, tile_json] : tiles_iter->items()) {
      if (auto tile = parse_tileset_tile(tile_json, tileset)) {
        tileset.tiles.push_back(std::move(*tile));
      }
      else {
        return propagate_unexpected(tile);
      }
    }
  }

  return kSuccess;
}

auto parse_tileset(const JSON& json, const SaveFormatReadOptions& options)
    -> Result<ir::Tileset>
{
  ir::Tileset tileset {};

  String relative_image_path {};

  return parse(json, "name", tileset.meta.name)
      .and_then([&] { return parse(json, "tilewidth", tileset.tile_width); })
      .and_then([&] { return parse(json, "tileheight", tileset.tile_height); })
      .and_then([&] { return parse(json, "tilecount", tileset.tile_count); })
      .and_then([&] { return parse(json, "columns", tileset.column_count); })
      .and_then([&] { return parse(json, "imagewidth", tileset.image_width); })
      .and_then([&] { return parse(json, "imageheight", tileset.image_height); })
      .and_then([&] { return parse(json, "image", relative_image_path); })
      .and_then([&]() -> Result<void> {
        // TODO make it possible to recover from missing tileset images
        if (options.strict_mode &&
            !std::filesystem::exists(options.base_dir / relative_image_path)) {
          TACTILE_LOG_ERROR("[TMJ] Tileset references non-existent image file: {}",
                            relative_image_path);
          return error(SaveFormatError::kBadFile);
        }

        tileset.image_path = std::move(relative_image_path);
        return kSuccess;
      })
      .and_then([&] { return parse_tileset_tiles(json, tileset); })
      .and_then([&] { return parse_metadata(json, tileset.meta); })
      .transform([&] { return std::move(tileset); });
}

auto parse_tileset_ref(const JSON& json, const SaveFormatReadOptions& options)
    -> Result<ir::TilesetRef>
{
  ir::TilesetRef tileset_ref {};

  const auto source_iter = json.find("source");
  const auto is_external_tileset = source_iter != json.end();

  if (is_external_tileset) {
    const auto& source = source_iter->get<String>();
    const auto tileset_path = options.base_dir / source;
    TACTILE_LOG_TRACE("[TMJ] Loading external tileset file {}...", tileset_path);

    const auto parsed_tileset_json = parse_json(tileset_path);
    if (!parsed_tileset_json) {
      TACTILE_LOG_ERROR("[TMJ] Could not read external tileset file {}", tileset_path);
      return propagate_unexpected(parsed_tileset_json);
    }

    return parse(json, "firstgid", tileset_ref.first_tile_id.value)
        .and_then([&] { return parse_tileset(*parsed_tileset_json, options); })
        .transform([&](ir::Tileset tileset) {
          tileset_ref.tileset = std::move(tileset);
          return std::move(tileset_ref);
        });
  }
  else {
    return parse(json, "firstgid", tileset_ref.first_tile_id.value)
        .and_then([&] { return parse_tileset(json, options); })
        .transform([&](ir::Tileset tileset) {
          tileset_ref.tileset = std::move(tileset);
          return std::move(tileset_ref);
        });
  }
}

auto parse_tilesets(const JSON& map_json,
                    const SaveFormatReadOptions& options,
                    ir::Map& map) -> Result<void>
{
  const auto tilesets_iter = map_json.find("tilesets");
  if (tilesets_iter == map_json.end()) {
    TACTILE_LOG_WARN("[TMJ] Map is missing required 'tilesets' attribute");
    return kSuccess;
  }

  map.tilesets.reserve(tilesets_iter->size());

  for (const auto& [_, tileset_json] : tilesets_iter->items()) {
    if (auto tileset_ref = parse_tileset_ref(tileset_json, options)) {
      map.tilesets.push_back(std::move(*tileset_ref));
    }
    else {
      return propagate_unexpected(tileset_ref);
    }
  }

  return kSuccess;
}

}  // namespace tactile::tmj
