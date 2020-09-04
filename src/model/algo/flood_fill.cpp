#include "flood_fill.hpp"

#include <array>    // array
#include <cstddef>  // byte
#include <deque>    // deque
#include <memory_resource>
#include <queue>  // queue

#include "map_position.hpp"

namespace tactile::model {

void flood_fill(layer& layer,
                const map_position& origin,
                tile_id target,
                tile_id replacement)
{
  const bool returnImmediately = !layer.in_bounds(origin) ||
                                 (target == replacement) ||
                                 (layer.tile_at(origin) != target);
  if (returnImmediately) {
    return;
  }

  layer.set_tile(origin, replacement);

  // 3,200 bytes given that map_position is 8 bytes
  using buffer_t = std::array<std::byte, 400 * sizeof(map_position)>;

  // TODO profile with VS and compare with unsynchronized_pool_resource
  using resource_t = std::pmr::monotonic_buffer_resource;
  using pmr_queue = std::queue<map_position, std::pmr::deque<map_position>>;

  buffer_t buffer{};
  resource_t resource{buffer.data(), sizeof buffer};
  pmr_queue queue{&resource};

  queue.push(origin);

  const auto update = [&](const map_position& position) {
    if (const auto tile = layer.tile_at(position); tile && tile == target) {
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