// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "bucket_tool.hpp"

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/tile/tileset_bundle.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"

namespace tactile {

void BucketTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void BucketTool::on_pressed(DocumentModel& model,
                            entt::dispatcher& dispatcher,
                            const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    const auto& document = model.require_active_map_document();
    const auto& map = document.get_map();
    const auto& tileset_bundle = map.get_tileset_bundle();

    const auto tileset_id = tileset_bundle.get_active_tileset_id().value();
    const auto& tileset_ref = tileset_bundle.get_tileset_ref(tileset_id);
    const auto& tileset = tileset_ref.get_tileset();

    const auto selected_pos = tileset_ref.get_selection()->begin;
    const auto replacement =
        tileset_ref.get_first_tile() + tileset.index_of(selected_pos);

    const auto layer_id = map.get_active_layer_id().value();
    dispatcher.enqueue<FloodEvent>(layer_id, mouse.position_in_viewport, replacement);
  }
}

auto BucketTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map_document();
  const auto& map = document.get_map();

  const auto& tilesets = map.get_tileset_bundle();
  const auto tileset_id = tilesets.get_active_tileset_id();

  return map.is_active_layer(LayerType::TileLayer) &&  //
         tileset_id && tilesets.get_tileset_ref(*tileset_id).is_single_tile_selected();
}

}  // namespace tactile
