#include "bucket_tool_system.hpp"

#include "assert.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const mouse_info& mouse)
{
  if (is_tile_layer_active(registry) && is_single_tile_selected_in_tileset(registry) &&
      mouse.button == cen::mouse_button::left) {
    const auto entity = find_active_tileset(registry);
    TACTILE_ASSERT(entity != entt::null);

    const auto& selection = registry.get<comp::tileset_selection>(entity);
    const auto position = selection.region->begin;
    const auto replacement = get_tile_from_tileset(registry, entity, position);

    dispatcher.enqueue<FloodEvent>(mouse.position_in_viewport, replacement);
  }
}

}  // namespace tactile::sys
