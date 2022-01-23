#include "stamp_tool_system.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/components/tileset.hpp"
#include "core/map_position.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tile_cache.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {
namespace {

inline TileCache gOldState;
inline TileCache gSequence;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return IsTileLayerActive(registry) && is_tileset_selection_not_empty(registry);
}

void UpdateSequence(entt::registry& registry, const MapPosition& cursor)
{
  TACTILE_ASSERT(IsUsable(registry));

  const auto layerEntity = GetActiveLayer(registry);
  TACTILE_ASSERT(layerEntity != entt::null);

  const auto tilesetEntity = find_active_tileset(registry);
  TACTILE_ASSERT(tilesetEntity != entt::null);

  const auto& selection = registry.get<TilesetSelection>(tilesetEntity);
  const auto& region = selection.region.value();
  const auto selectionSize = region.end - region.begin;
  const auto previewOffset = selectionSize / MapPosition{2, 2};
  const auto endRow = selectionSize.GetRow();
  const auto endCol = selectionSize.GetColumn();

  for (auto row = 0; row < endRow; ++row) {
    for (auto col = 0; col < endCol; ++col) {
      const auto index = MapPosition{row, col};
      const auto selectionPosition = region.begin + index;

      const auto tile = get_tile_from_tileset(registry, tilesetEntity, selectionPosition);
      if (tile != empty_tile) {
        const auto pos = cursor + index - previewOffset;
        if (IsPositionInMap(registry, pos)) {
          if (!gOldState.contains(pos)) {
            gOldState.emplace(pos, GetTileFromLayer(registry, layerEntity, pos));
          }
          gSequence.emplace_or_replace(pos, tile);
          SetTileInLayer(registry, layerEntity, pos, tile);
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
