#include "flood_fill.hpp"

#include <queue>  // queue

#include "core/systems/map_system.hpp"
#include "core/systems/tile_layer_system.hpp"

namespace Tactile {

void FloodFill(entt::registry& registry,
               const entt::entity entity,
               const MapPosition& origin,
               const tile_id replacement,
               std::vector<MapPosition>& affected)
{
  const auto target = Sys::GetTileFromLayer(registry, entity, origin);

  if (!Sys::IsPositionInMap(registry, origin) || (target == replacement))
  {
    return;
  }

  std::queue<MapPosition> positions;

  Sys::SetTileInLayer(registry, entity, origin, replacement);
  affected.push_back(origin);
  positions.push(origin);

  auto update = [&](const MapPosition& position) {
    if (Sys::IsPositionInMap(registry, position))
    {
      const auto tile = Sys::GetTileFromLayer(registry, entity, position);
      if (tile == target)
      {
        Sys::SetTileInLayer(registry, entity, position, replacement);
        affected.push_back(position);
        positions.push(position);
      }
    }
  };

  while (!positions.empty())
  {
    const auto position = positions.front();
    positions.pop();

    update(position.West());
    update(position.East());
    update(position.South());
    update(position.North());
  }
}

}  // namespace Tactile
