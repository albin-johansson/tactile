/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "map_command_cache.hpp"

#include "core/layer/group_layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"

namespace tactile {

struct SaveTilesVisitor final : ConstLayerVisitor {
  MapCommandCache* self {};
  TilePos begin;
  TilePos end;

  void visit(const TileLayer& layer) override
  {
    auto& tile_cache = self->mCache[layer.get_uuid()];

    const auto end_row = end.row();
    const auto end_col = end.col();
    for (auto row = begin.row(); row < end_row; ++row) {
      for (auto col = begin.col(); col < end_col; ++col) {
        const TilePos position {row, col};
        const auto tile_id = layer.tile_at(position).value();
        tile_cache.try_emplace(position, tile_id);
      }
    }
  }
};

void MapCommandCache::clear() noexcept
{
  mCache.clear();
}

void MapCommandCache::restore_tiles(Map& map)
{
  auto& root = map.invisible_root();
  for (const auto& [layer_id, tile_cache]: mCache) {
    auto& layer = root.get_tile_layer(layer_id);

    for (const auto& [position, tile_id]: tile_cache) {
      layer.set_tile(position, tile_id);
    }
  }
}

void MapCommandCache::save_tiles(const Map& map, const TilePos& begin, const TilePos& end)
{
  SaveTilesVisitor visitor;
  visitor.self = this;
  visitor.begin = begin;
  visitor.end = end;
  map.invisible_root().each(visitor);
}

void MapCommandCache::merge_with(const MapCommandCache& other)
{
  for (const auto& [other_layer, other_tile_cache]: other.mCache) {
    auto& tile_cache = mCache[other_layer];
    for (const auto& [other_pos, other_tile]: other_tile_cache) {
      tile_cache.try_emplace(other_pos, other_tile);
    }
  }
}

}  // namespace tactile
