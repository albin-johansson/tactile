#include "flood_fill.hpp"

#include <array>  // array
#include <queue>  // queue

#include "map_position.hpp"

namespace tactile::core {

void FloodFill(TileLayer& layer,
               const MapPosition& origin,
               tile_id replacement,
               std::vector<MapPosition>& affected)
{
  const auto target = layer.TileAt(origin);

  if (!layer.InBounds(origin) || (target == replacement))
  {
    return;
  }

  layer.SetTile(origin, replacement);

  std::queue<MapPosition> queue;

  affected.push_back(origin);
  queue.push(origin);

  const auto update = [&](const MapPosition& pos) {
    if (const auto tile = layer.TileAt(pos); tile && tile == target)
    {
      affected.push_back(pos);
      layer.SetTile(pos, replacement);
      queue.push(pos);
    }
  };

  while (!queue.empty())
  {
    const auto pos = queue.front();
    queue.pop();

    update(pos.West());
    update(pos.East());
    update(pos.South());
    update(pos.North());
  }
}

}  // namespace tactile::core