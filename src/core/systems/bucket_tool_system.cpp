#include "bucket_tool_system.hpp"

#include <cassert>  // assert

#include "core/components/tileset.hpp"
#include "events/flood_event.hpp"
#include "layer_system.hpp"
#include "tileset_system.hpp"

namespace Tactile::Sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse)
{
  if (Sys::IsTileLayerActive(registry) &&
      Sys::IsSingleTileSelectedInTileset(registry) &&
      mouse.button == cen::mouse_button::left)
  {
    const auto entity = Sys::GetActiveTileset(registry);
    assert(entity != entt::null);

    const auto& selection = registry.get<TilesetSelection>(entity);
    const auto position = selection.region->begin;
    const auto replacement = Sys::GetTileFromTileset(registry, entity, position);

    dispatcher.enqueue<FloodEvent>(mouse.position_in_map, replacement);
  }
}

}  // namespace Tactile::Sys
