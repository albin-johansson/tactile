// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_tileset_parser.hpp"

#include <fstream>  // ifstream
#include <utility>  // move

#include "tactile/base/io/save/save_format_parse_result.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"
#include "tactile/tiled_tmj_format/tmj_format_layer_parser.hpp"

namespace tactile {
namespace tmj_format_tileset_parser {

[[nodiscard]]
auto parse_animation_frame(const nlohmann::json& frame_json)
    -> SaveFormatParseResult<ir::AnimationFrame>
{
  ir::AnimationFrame frame {};

  if (const auto frame_tile_iter = frame_json.find("tileid");
      frame_tile_iter != frame_json.end()) {
    frame_tile_iter->get_to(frame.tile_index);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoAnimationFrameTileIndex};
  }

  if (const auto duration_iter = frame_json.find("duration");
      duration_iter != frame_json.end()) {
    frame.duration = Milliseconds {duration_iter->get<Milliseconds::rep>()};
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoAnimationFrameTileIndex};
  }

  return frame;
}

[[nodiscard]]
auto parse_tile_animation(const nlohmann::json& animation_json)
    -> SaveFormatParseResult<ir::TileAnimation>
{
  ir::TileAnimation animation {};
  animation.reserve(animation_json.size());

  for (const auto& [_, frame_json] : animation_json.items()) {
    if (const auto frame = parse_animation_frame(frame_json)) {
      animation.push_back(*frame);
    }
    else {
      return std::unexpected {frame.error()};
    }
  }

  return animation;
}

[[nodiscard]]
auto parse_tile(const nlohmann::json& tile_json) -> SaveFormatParseResult<ir::Tile>
{
  ir::Tile tile {};

  if (auto metadata = parse_tiled_tmj_metadata(tile_json)) {
    tile.meta = std::move(*metadata);
  }
  else {
    return std::unexpected {metadata.error()};
  }

  if (const auto index_iter = tile_json.find("id"); index_iter != tile_json.end()) {
    index_iter->get_to(tile.index);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTileIndex};
  }

  if (const auto object_layer_iter = tile_json.find("objectgroup");
      object_layer_iter != tile_json.end()) {
    ir::Layer object_layer {};
    if (const auto result = parse_tiled_tmj_object_layer(*object_layer_iter, object_layer)) {
      tile.objects = std::move(object_layer.objects);
    }
    else {
      return std::unexpected {result.error()};
    }
  }

  if (const auto animation_iter = tile_json.find("animation");
      animation_iter != tile_json.end()) {
    if (auto animation = parse_tile_animation(*animation_iter)) {
      tile.animation = std::move(*animation);
    }
    else {
      return std::unexpected {animation.error()};
    }
  }

  return tile;
}

[[nodiscard]]
auto parse_tileset(const nlohmann::json& tileset_json) -> SaveFormatParseResult<ir::Tileset>
{
  if (!tileset_json.contains("name")) {
    return std::unexpected {SaveFormatParseError::kNoTilesetName};
  }

  ir::Tileset tileset {};

  if (auto metadata = parse_tiled_tmj_metadata(tileset_json)) {
    tileset.meta = std::move(*metadata);
  }

  if (const auto tile_width_iter = tileset_json.find("tilewidth");
      tile_width_iter != tileset_json.end()) {
    tile_width_iter->get_to(tileset.tile_size[0]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetTileWidth};
  }

  if (const auto tile_height_iter = tileset_json.find("tileheight");
      tile_height_iter != tileset_json.end()) {
    tile_height_iter->get_to(tileset.tile_size[1]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetTileHeight};
  }

  if (const auto tile_count_iter = tileset_json.find("tilecount");
      tile_count_iter != tileset_json.end()) {
    tile_count_iter->get_to(tileset.tile_count);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetTileCount};
  }

  if (const auto columns_iter = tileset_json.find("columns");
      columns_iter != tileset_json.end()) {
    columns_iter->get_to(tileset.column_count);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetColumns};
  }

  if (const auto image_width_iter = tileset_json.find("imagewidth");
      image_width_iter != tileset_json.end()) {
    image_width_iter->get_to(tileset.image_size[0]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetImageWidth};
  }

  if (const auto image_height_iter = tileset_json.find("imageheight");
      image_height_iter != tileset_json.end()) {
    image_height_iter->get_to(tileset.image_size[1]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetImageHeight};
  }

  std::string relative_image_path {};
  if (const auto image_path_iter = tileset_json.find("image");
      image_path_iter != tileset_json.end()) {
    image_path_iter->get_to(relative_image_path);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetImage};
  }

  tileset.image_path = std::filesystem::path {relative_image_path};

  if (const auto tiles_iter = tileset_json.find("tiles"); tiles_iter != tileset_json.end()) {
    tileset.tiles.reserve(tiles_iter->size());

    for (const auto& [_, tile_json] : tiles_iter->items()) {
      if (auto tile = tmj_format_tileset_parser::parse_tile(tile_json)) {
        tileset.tiles.push_back(std::move(*tile));
      }
      else {
        return std::unexpected {tile.error()};
      }
    }
  }

  return tileset;
}

}  // namespace tmj_format_tileset_parser

auto parse_tiled_tmj_tileset(const nlohmann::json& tileset_json,
                             const SaveFormatReadOptions& options)
    -> SaveFormatParseResult<ir::TilesetRef>
{
  ir::TilesetRef tileset_ref {};

  if (const auto first_gid_iter = tileset_json.find("firstgid");
      first_gid_iter != tileset_json.end()) {
    first_gid_iter->get_to(tileset_ref.first_tile_id);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTilesetFirstTileId};
  }

  if (const auto source_iter = tileset_json.find("source");
      source_iter != tileset_json.end()) {
    const auto& source = source_iter->get_ref<const std::string&>();

    const auto tileset_path = options.base_dir / source;
    log(LogLevel::kDebug, "Loading external tileset: {}", tileset_path.string());

    std::ifstream stream {tileset_path, std::ios::in};
    nlohmann::json external_tileset_json {};
    stream >> external_tileset_json;

    if (auto tileset = tmj_format_tileset_parser::parse_tileset(external_tileset_json)) {
      tileset_ref.tileset = std::move(*tileset);
      tileset_ref.tileset.is_embedded = false;
    }
    else {
      return std::unexpected {tileset.error()};
    }
  }
  else {
    if (auto tileset = tmj_format_tileset_parser::parse_tileset(tileset_json)) {
      tileset_ref.tileset = std::move(*tileset);
      tileset_ref.tileset.is_embedded = true;
    }
    else {
      return std::unexpected {tileset.error()};
    }
  }

  return tileset_ref;
}

}  // namespace tactile
