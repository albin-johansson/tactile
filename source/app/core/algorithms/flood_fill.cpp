#include "flood_fill.hpp"

#include <queue>  // queue

#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

void flood(entt::registry& registry,
           const entt::entity entity,
           const tile_position& origin,
           const tile_id replacement,
           std::vector<tile_position>& affected)
{
  auto& layer = registry.get<comp::tile_layer>(entity);
  const auto target = sys::get_tile(layer, origin);

  if (!sys::is_position_in_map(registry, origin) || (target == replacement)) {
    return;
  }

  std::queue<tile_position> positions;
  positions.push(origin);

  sys::set_tile(layer, origin, replacement);
  affected.push_back(origin);

  auto update = [&](const tile_position& position) {
    if (sys::is_position_in_map(registry, position)) {
      const auto tile = sys::get_tile(layer, position);
      if (tile == target) {
        sys::set_tile(layer, position, replacement);
        affected.push_back(position);
        positions.push(position);
      }
    }
  };

  while (!positions.empty()) {
    const auto position = positions.front();
    positions.pop();

    update(position.west());
    update(position.east());
    update(position.south());
    update(position.north());
  }
}

}  // namespace tactile
