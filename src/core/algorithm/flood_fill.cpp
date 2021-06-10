#include "flood_fill.hpp"

#include <array>  // array
#include <queue>  // queue

#include "position.hpp"

namespace tactile::core {

void FloodFill(tile_layer& layer,
               const position& origin,
               tile_id replacement,
               std::vector<position>& affected)
{
  const auto target = layer.tile_at(origin);

  if (!layer.in_bounds(origin) || (target == replacement))
  {
    return;
  }

  layer.set_tile(origin, replacement);

  std::queue<position> queue;

  affected.push_back(origin);
  queue.push(origin);

  const auto update = [&](const position& pos) {
    if (const auto tile = layer.tile_at(pos); tile && tile == target)
    {
      affected.push_back(pos);
      layer.set_tile(pos, replacement);
      queue.push(pos);
    }
  };

  while (!queue.empty())
  {
    const auto pos = queue.front();
    queue.pop();

    update(pos.west());
    update(pos.east());
    update(pos.south());
    update(pos.north());
  }
}

}  // namespace tactile::core