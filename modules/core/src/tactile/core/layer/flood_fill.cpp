// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/flood_fill.hpp"

#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/foundation/container/queue.hpp"

namespace tactile {

void flood_fill(TileLayer& layer,
                const TilePos& start_pos,
                TileID new_id,
                Vector<TilePos>* affected_positions)
{
  if (!layer.is_valid_pos(start_pos) || layer.tile_at(start_pos) == new_id) {
    return;
  }

  const auto target_id = layer.tile_at(start_pos);

  Queue<TilePos> remaining_positions {};

  auto maybe_update_tile_at = [&](const TilePos& tile_pos) {
    if (layer.tile_at(tile_pos) == target_id) {
      layer.set_tile(tile_pos, new_id);

      if (affected_positions != nullptr) {
        affected_positions->push_back(tile_pos);
      }

      remaining_positions.push(tile_pos);
    }
  };

  maybe_update_tile_at(start_pos);

  while (!remaining_positions.empty()) {
    const auto current_position = remaining_positions.front();
    remaining_positions.pop();

    maybe_update_tile_at(current_position - TilePos {0, 1});
    maybe_update_tile_at(current_position + TilePos {0, 1});
    maybe_update_tile_at(current_position - TilePos {1, 0});
    maybe_update_tile_at(current_position + TilePos {1, 0});
  }
}

}  // namespace tactile
