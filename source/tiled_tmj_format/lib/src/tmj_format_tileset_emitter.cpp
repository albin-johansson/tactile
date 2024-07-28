// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_tileset_emitter.hpp"

#include <filesystem>  // relative
#include <format>      // format
#include <utility>     // move

#include "tactile/base/document/tileset_view.hpp"
#include "tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"

namespace tactile {
namespace tmj_format_tileset_emitter {

void save_basic_tileset_attributes(const ITilesetView& tileset,
                                   nlohmann::json& tileset_json,
                                   const SaveFormatWriteOptions& options)
{
  const auto tile_size = tileset.get_tile_size();
  const auto image_size = tileset.get_image_size();

  const auto relative_image_path =
      std::filesystem::relative(tileset.get_image_path(), options.base_dir);

  tileset_json["columns"] = tileset.column_count();

  tileset_json["tilewidth"] = tile_size.x();
  tileset_json["tileheight"] = tile_size.y();
  tileset_json["tilecount"] = tileset.tile_count();

  tileset_json["image"] = relative_image_path.string();
  tileset_json["imagewidth"] = image_size.x();
  tileset_json["imageheight"] = image_size.y();

  tileset_json["margin"] = 0;
  tileset_json["spacing"] = 0;

  const auto tile_definition_count = tileset.tile_definition_count();
  if (tile_definition_count > 0) {
    auto tile_array = nlohmann::json::array();
    tile_array.get_ref<nlohmann::json::array_t&>().reserve(tile_definition_count);
    tileset_json["tiles"] = std::move(tile_array);
  }

  emit_tiled_tmj_metadata(tileset.get_meta(), tileset_json);
}

}  // namespace tmj_format_tileset_emitter

auto emit_tiled_tmj_tileset(const ITilesetView& tileset,
                            const SaveFormatWriteOptions& options,
                            HashMap<TileID, nlohmann::json>& external_tilesets)
    -> nlohmann::json
{
  auto tileset_json = nlohmann::json::object();
  tileset_json["firstgid"] = tileset.get_first_tile_id();

  if (options.use_external_tilesets) {
    tileset_json["source"] = std::format("{}.tsj", tileset.get_filename());

    auto external_tileset_json = nlohmann::json::object();
    tmj_format_tileset_emitter::save_basic_tileset_attributes(tileset,
                                                              external_tileset_json,
                                                              options);

    external_tilesets[tileset.get_first_tile_id()] = std::move(external_tileset_json);
  }
  else {
    tmj_format_tileset_emitter::save_basic_tileset_attributes(tileset, tileset_json, options);
  }

  return tileset_json;
}

}  // namespace tactile
