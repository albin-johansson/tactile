#include "eraser_tool_system.hpp"

#include <cassert>              // assert
#include <rune/everything.hpp>  // vector_map
#include <utility>              // move

#include "aliases/tile_cache.hpp"
#include "aliases/tile_id.hpp"
#include "core/map_position.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "events/tool_events.hpp"

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

  // TODO rune::vector_map::try_emplace
  if (!old_state.contains(cursor)) {
    old_state.emplace(cursor, GetTileFromLayer(registry, entity, cursor));
  }

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
