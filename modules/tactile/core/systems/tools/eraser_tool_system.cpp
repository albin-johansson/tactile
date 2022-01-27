#include "eraser_tool_system.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {
namespace {

inline TileCache gOldState;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return IsTileLayerActive(registry);
}

void UpdateSequence(entt::registry& registry, const tile_position& cursor)
{
  const auto entity = GetActiveLayer(registry);
  TACTILE_ASSERT(entity != entt::null);

  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));
  auto& layer = registry.get<TileLayer>(entity);

  if (!gOldState.contains(cursor)) {
    gOldState.emplace(cursor, get_tile(layer, cursor));
  }

  set_tile(layer, cursor, empty_tile);
}

}  // namespace

void EraserToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    gOldState.clear();
    UpdateSequence(registry, mouse.position_in_map);
  }
}

void EraserToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    UpdateSequence(registry, mouse.position_in_map);
  }
}

void EraserToolOnReleased(entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    dispatcher.enqueue<EraserSequenceEvent>(std::move(gOldState));
  }
}

}  // namespace tactile::sys
