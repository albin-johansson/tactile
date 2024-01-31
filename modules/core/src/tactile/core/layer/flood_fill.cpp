// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/flood_fill.hpp"

#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/foundation/container/queue.hpp"
#include "tactile/foundation/debug/profiler.hpp"

namespace tactile {
namespace {

struct FloodFillSpan final {
  ssize min_x;
  ssize max_x;
  ssize y;
  ssize dy;
};

}  // namespace

void flood_fill(TileLayer& layer,
                const TilePos& start_pos,
                const TileID new_id,
                Vector<TilePos>* affected_positions)
{
  TACTILE_DEBUG_PROFILE_SCOPE("flood_fill");

  const auto target_id = layer.tile_at(start_pos);

  if (!layer.is_valid_pos(start_pos) || target_id == new_id) {
    return;
  }

  auto inside = [&, target_id](const ssize x, const ssize y) {
    return layer.tile_at(TilePos {y, x}) == target_id;
  };

  auto try_set = [&, new_id](const ssize x, const ssize y) {
    if (inside(x, y) && layer.set_tile(TilePos {y, x}, new_id)) {
      if (affected_positions) {
        affected_positions->push_back(TilePos {y, x});
      }

      return true;
    }

    return false;
  };

  Queue<FloodFillSpan> spans {};
  spans.push(FloodFillSpan {start_pos.col, start_pos.col, start_pos.row, 1});
  spans.push(FloodFillSpan {start_pos.col, start_pos.col, start_pos.row - 1, -1});

  while (!spans.empty()) {
    const auto span = spans.front();
    spans.pop();

    auto cursor_x = span.min_x;
    auto min_x = cursor_x;
    auto max_x = cursor_x;

    if (inside(cursor_x, span.y)) {
      while (try_set(cursor_x - 1, span.y)) {
        --cursor_x;
      }

      min_x = cursor_x;

      if (min_x < span.min_x) {
        spans.push(FloodFillSpan {min_x, span.min_x - 1, span.y - span.dy, -span.dy});
      }
    }

    cursor_x = max_x;

    while (cursor_x <= span.max_x) {
      while (try_set(cursor_x, span.y)) {
        ++cursor_x;
      }

      max_x = cursor_x;

      if (max_x > min_x) {
        spans.push(FloodFillSpan {min_x, max_x - 1, span.y + span.dy, span.dy});
      }

      if (max_x - 1 > span.max_x) {
        spans.push(FloodFillSpan {span.max_x, max_x - 1, span.y - span.dy, -span.dy});
      }

      ++cursor_x;
      while (cursor_x < span.max_x && !inside(cursor_x, span.y)) {
        ++cursor_x;
      }

      min_x = cursor_x;
    }
  }
}

}  // namespace tactile
