#include "eraser_tool_system.hpp"

#include <utility>  // move

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "editor/events/tool_events.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile::sys {
namespace {

inline TileCache gOldState;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return is_tile_layer_active(registry);
}

void UpdateSequence(entt::registry& registry, const tile_position& cursor)
{
  const auto entity = get_active_layer(registry);
  TACTILE_ASSERT(entity != entt::null);

  TACTILE_ASSERT(registry.all_of<comp::tile_layer>(entity));
  auto& layer = registry.get<comp::tile_layer>(entity);

  if (!gOldState.contains(cursor)) {
    gOldState.emplace(cursor, get_tile(layer, cursor));
  }

  set_tile(layer, cursor, empty_tile);
}

}  // namespace

void EraserToolOnPressed(entt::registry& registry, const mouse_info& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    gOldState.clear();
    UpdateSequence(registry, mouse.position_in_viewport);
  }
}

void EraserToolOnDragged(entt::registry& registry, const mouse_info& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    UpdateSequence(registry, mouse.position_in_viewport);
  }
}

void EraserToolOnReleased(entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const mouse_info& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    dispatcher.enqueue<eraser_sequence_event>(std::move(gOldState));
  }
}

}  // namespace tactile::sys
