#include "stamp_tool_system.hpp"

#include <cassert>              // assert
#include <rune/everything.hpp>  // vector_map
#include <utility>              // move

#include "aliases/tile_id.hpp"
#include "core/components/tile_layer.hpp"
#include "core/components/tileset.hpp"
#include "core/map_position.hpp"
#include "events/stamp_sequence_event.hpp"
#include "layer_system.hpp"
#include "map_system.hpp"
#include "tile_layer_system.hpp"
#include "tileset_system.hpp"

namespace Tactile::Sys {
namespace {

inline rune::vector_map<MapPosition, tile_id> old_state;
inline rune::vector_map<MapPosition, tile_id> sequence;

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return Sys::IsTileLayerActive(registry) &&
         Sys::HasNonEmptyTilesetSelection(registry);
}

void UpdateSequence(entt::registry& registry, const MapPosition& cursor)
{
  assert(IsUsable(registry));

  const auto layerEntity = Sys::GetActiveLayer(registry);
  assert(layerEntity != entt::null);

  const auto tilesetEntity = Sys::GetActiveTileset(registry);
  assert(tilesetEntity != entt::null);

  const auto& selection = registry.get<TilesetSelection>(tilesetEntity);
  const auto& region = selection.region.value();
  const auto selectionSize = region.end - region.begin;
  const auto previewOffset = selectionSize / MapPosition{2_row, 2_col};
  const auto endRow = selectionSize.GetRow();
  const auto endCol = selectionSize.GetColumn();

  auto& tileLayer = registry.get<TileLayer>(layerEntity);
  for (auto row = 0_row; row < endRow; ++row)
  {
    for (auto col = 0_col; col < endCol; ++col)
    {
      const auto index = MapPosition{row, col};
      const auto selectionPosition = region.begin + index;

      const auto tile =
          Sys::GetTileFromTileset(registry, tilesetEntity, selectionPosition);
      if (tile != empty_tile)
      {
        const auto pos = cursor + index - previewOffset;
        if (Sys::IsPositionInMap(registry, pos))
        {
          if (!old_state.contains(pos))
          {
            const auto prev = Sys::GetTileFromLayer(registry, layerEntity, pos);
            old_state.emplace(pos, prev);
          }
          sequence.emplace_or_replace(pos, tile);
          tileLayer.matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = tile;
        }
      }
    }
  }
}

}  // namespace

void StampToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left)
  {
    UpdateSequence(registry, mouse.position_in_map);
  }
}

void StampToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left)
  {
    UpdateSequence(registry, mouse.position_in_map);
  }
}

void StampToolOnReleased(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left)
  {
    dispatcher.enqueue<StampSequenceEvent>(std::move(old_state),
                                           std::move(sequence));
    old_state.clear();
    sequence.clear();
  }
}

}  // namespace Tactile::Sys
