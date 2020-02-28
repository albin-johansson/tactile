#include "flood_fill.h"

#include <queue>

#include "map_position.h"

namespace tactile {
namespace {

/**
 * Updates the tile layer and queue if the supplied position should be
 * affected by the flood fill.
 *
 * @param layer the tile layer in which the flood fill is being run.
 * @param position the position in the tile layer that will be checked.
 * @param target the target tile type.
 * @param replacement the replacement tile type.
 * @param queue the queue that will be updated.
 * @since 0.1.0
 */
void update(TileLayer& layer,
            const MapPosition& position,
            TileID target,
            TileID replacement,
            std::queue<MapPosition>& queue) noexcept
{
  const auto tile = layer.tile_at(position);
  if (tile && tile == target) {
    layer.set_tile(position, replacement);
    queue.push(position);
  }
}

}  // namespace

void flood_fill(TileLayer& layer,
                const MapPosition& origin,
                TileID target,
                TileID replacement) noexcept
{
  const bool returnImmediately = !layer.in_bounds(origin) ||
                                 (target == replacement) ||
                                 (layer.tile_at(origin) != target);
  if (returnImmediately) {
    return;
  }

  layer.set_tile(origin, replacement);

  std::queue<MapPosition> queue;
  queue.push(origin);

  while (!queue.empty()) {
    const auto pos = queue.front();
    queue.pop();

    update(layer, pos.west(), target, replacement, queue);
    update(layer, pos.east(), target, replacement, queue);
    update(layer, pos.south(), target, replacement, queue);
    update(layer, pos.north(), target, replacement, queue);
  }
}

}  // namespace tactile