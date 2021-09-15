#include "bucket_tool_system.hpp"

#include <cassert>  // assert

#include "core/components/tileset.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "events/tool_events.hpp"

namespace Tactile::Sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse)
{
  if (IsTileLayerActive(registry) && IsSingleTileSelectedInTileset(registry) &&
      mouse.button == cen::mouse_button::left)
  {
    const auto entity = GetActiveTileset(registry);
    assert(entity != entt::null);

    const auto& selection = registry.get<TilesetSelection>(entity);
    const auto position = selection.region->begin;
    const auto replacement = GetTileFromTileset(registry, entity, position);

    dispatcher.enqueue<FloodEvent>(mouse.position_in_map, replacement);
  }
}

}  // namespace Tactile::Sys
