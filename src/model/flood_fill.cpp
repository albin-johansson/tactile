#include "flood_fill.hpp"

#include <queue>

#include "map_position.hpp"

namespace tactile::model {

void flood_fill(tile_layer& layer,
                const map_position& origin,
                tile_id target,
                tile_id replacement) noexcept
{
  const bool returnImmediately = !layer.in_bounds(origin) ||
                                 (target == replacement) ||
                                 (layer.tile_at(origin) != target);
  if (returnImmediately) {
    return;
  }

  layer.set_tile(origin, replacement);

  std::queue<map_position> queue;
  queue.push(origin);

  const auto update = [&layer, &queue, target, replacement](
                          const map_position& position) noexcept {
    const auto tile = layer.tile_at(position);
    if (tile && tile == target) {
      layer.set_tile(position, replacement);
      queue.push(position);
    }
  };

  while (!queue.empty()) {
    const auto pos = queue.front();
    queue.pop();

    update(pos.west());
    update(pos.east());
    update(pos.south());
    update(pos.north());
  }
}

}  // namespace tactile::model