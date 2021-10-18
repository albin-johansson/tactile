#include "stamp_tool_system.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include <rune/rune.hpp>  // vector_map

#include <tactile-base/map_position.hpp>
#include "common/tile_cache.hpp"
#include "core/components/tile_layer.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/events/tool_events.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile::Sys {
namespace {

inline TileCache old_state;
inline TileCache sequence;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return IsTileLayerActive(registry) && HasNonEmptyTilesetSelection(registry);
}

void UpdateSequence(entt::registry& registry, const MapPosition& cursor)
{
  assert(IsUsable(registry));

  const auto layerEntity = GetActiveLayer(registry);
  assert(layerEntity != entt::null);

  const auto tilesetEntity = GetActiveTileset(registry);
  assert(tilesetEntity != entt::null);

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

      const auto tile = GetTileFromTileset(registry, tilesetEntity, selectionPosition);
      if (tile != empty_tile) {
        const auto pos = cursor + index - previewOffset;
        if (IsPositionInMap(registry, pos)) {
          old_state.try_emplace(pos, GetTileFromLayer(registry, layerEntity, pos));
          sequence.emplace_or_replace(pos, tile);
          Sys::SetTileInLayer(registry, layerEntity, pos, tile);
        }
      }
    }
  }
}

}  // namespace

void StampToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    old_state.clear();
    sequence.clear();

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
    dispatcher.enqueue<StampSequenceEvent>(std::move(old_state), std::move(sequence));
  }
}

}  // namespace Tactile::Sys
