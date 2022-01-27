#include "stamp_tool_system.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {
namespace {

inline TileCache gOldState;
inline TileCache gSequence;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return is_tile_layer_active(registry) && is_tileset_selection_not_empty(registry);
}

void UpdateSequence(entt::registry& registry, const tile_position& cursor)
{
  TACTILE_ASSERT(IsUsable(registry));

  const auto layerEntity = get_active_layer(registry);
  TACTILE_ASSERT(layerEntity != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::tile_layer>(layerEntity));

  auto& layer = registry.get<comp::tile_layer>(layerEntity);

  const auto tilesetEntity = find_active_tileset(registry);
  TACTILE_ASSERT(tilesetEntity != entt::null);

  const auto& selection = registry.get<comp::tileset_selection>(tilesetEntity);
  const auto& region = selection.region.value();
  const auto selectionSize = region.end - region.begin;
  const auto previewOffset = selectionSize / tile_position{2, 2};
  const auto endRow = selectionSize.row();
  const auto endCol = selectionSize.col();

  for (auto row = 0; row < endRow; ++row) {
    for (auto col = 0; col < endCol; ++col) {
      const auto index = tile_position{row, col};
      const auto selectionPosition = region.begin + index;

      const auto tile = get_tile_from_tileset(registry, tilesetEntity, selectionPosition);
      if (tile != empty_tile) {
        const auto pos = cursor + index - previewOffset;
        if (is_position_in_map(registry, pos)) {
          if (!gOldState.contains(pos)) {
            gOldState.emplace(pos, get_tile(layer, pos));
          }
          gSequence.emplace_or_replace(pos, tile);
          set_tile(layer, pos, tile);
        }
      }
    }
  }
}

}  // namespace

void StampToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    gOldState.clear();
    gSequence.clear();

    UpdateSequence(registry, mouse.position_in_map);
  }
}

void StampToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    UpdateSequence(registry, mouse.position_in_map);
  }
}

void StampToolOnReleased(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    dispatcher.enqueue<StampSequenceEvent>(std::move(gOldState), std::move(gSequence));
  }
}

}  // namespace tactile::sys
