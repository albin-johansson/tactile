#include "stamp_tool_system.hpp"

#include <utility>  // move

#include "core/components/layer.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "editor/events/tool_events.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile::sys {
namespace {

inline TileCache _old_state;
inline TileCache _sequence;

[[nodiscard]] auto _is_usable(const entt::registry& registry) -> bool
{
  return is_tile_layer_active(registry) && is_tileset_selection_not_empty(registry);
}

void _update_sequence(entt::registry& registry, const tile_position& cursor)
{
  TACTILE_ASSERT(_is_usable(registry));

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
          if (!_old_state.contains(pos)) {
            _old_state.emplace(pos, get_tile(layer, pos));
          }
          _sequence.emplace_or_replace(pos, tile);
          set_tile(layer, pos, tile);
        }
      }
    }
  }
}

}  // namespace

void stamp_tool_on_pressed(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    _old_state.clear();
    _sequence.clear();

    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void stamp_tool_on_dragged(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void stamp_tool_on_released(entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    dispatcher.enqueue<stamp_sequence_event>(std::move(_old_state), std::move(_sequence));
  }
}

}  // namespace tactile::sys
