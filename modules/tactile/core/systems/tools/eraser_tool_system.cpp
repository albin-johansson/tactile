#include "eraser_tool_system.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include <rune/rune.hpp>  // vector_map

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

#include "common/tile_cache.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "editor/events/tool_events.hpp"

namespace Tactile::Sys {
namespace {

inline TileCache old_state;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return IsTileLayerActive(registry);
}

void UpdateSequence(entt::registry& registry, const MapPosition& cursor)
{
  const auto entity = GetActiveLayer(registry);
  assert(entity != entt::null);

  old_state.try_emplace(cursor, GetTileFromLayer(registry, entity, cursor));

  SetTileInLayer(registry, entity, cursor, empty_tile);
}

}  // namespace

void EraserToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    old_state.clear();
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
    dispatcher.enqueue<EraserSequenceEvent>(std::move(old_state));
  }
}

}  // namespace Tactile::Sys
