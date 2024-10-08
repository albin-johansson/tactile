// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_tile_emitter.hpp"

#include <cstddef>  // size_t
#include <utility>  // move

#include "tactile/base/document/tile_view.hpp"
#include "tactile/tiled_tmj/tmj_format_meta_emitter.hpp"

namespace tactile {
namespace tmj_format_tile_emitter {

void save_tile_animation(const ITileView& tile, nlohmann::json& tile_json)
{
  const auto frame_count = tile.animation_frame_count();
  if (frame_count == 0) {
    return;
  }

  auto animation_json = nlohmann::json::array();
  animation_json.get_ref<nlohmann::json::array_t&>().reserve(frame_count);

  for (std::size_t index = 0; index < frame_count; ++index) {
    auto frame_json = nlohmann::json::object();

    const auto [frame_tile, frame_duration] = tile.get_animation_frame(index);
    frame_json["tileid"] = frame_tile;
    frame_json["duration"] = frame_duration.count();

    animation_json.push_back(std::move(frame_json));
  }

  tile_json["animation"] = std::move(animation_json);
}

}  // namespace tmj_format_tile_emitter

auto emit_tiled_tmj_tile(const ITileView& tile) -> nlohmann::json
{
  auto tile_json = nlohmann::json::object();

  tile_json["id"] = tile.get_index();

  tmj_format_tile_emitter::save_tile_animation(tile, tile_json);

  if (tile.object_count() > 0) {
    auto object_array = nlohmann::json::array();
    object_array.get_ref<nlohmann::json::array_t&>().reserve(tile.object_count());

    auto dummy_layer = nlohmann::json::object();

    // Normal layers feature an "id" attribute, but Tiled seems to be fine without it here.
    dummy_layer["draworder"] = "index";
    dummy_layer["name"] = "";
    dummy_layer["opacity"] = 1;
    dummy_layer["type"] = "objectgroup";
    dummy_layer["visible"] = true;
    dummy_layer["x"] = 0;
    dummy_layer["y"] = 0;
    dummy_layer["objects"] = std::move(object_array);

    tile_json["objectgroup"] = std::move(dummy_layer);
  }

  emit_tiled_tmj_metadata(tile.get_meta(), tile_json);

  return tile_json;
}

}  // namespace tactile
