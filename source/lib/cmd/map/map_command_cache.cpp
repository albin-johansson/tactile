/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model/components/layer.hpp"
#include "model/components/map.hpp"
#include "model/systems/layer_system.hpp"

namespace tactile {

void MapCommandCache::clear() noexcept
{
  mCache.clear();
}

void MapCommandCache::restore_tiles(Model& model)
{
  for (const auto& [layer_entity, tile_cache]: mCache) {
    auto& layer = model.get<TileLayer>(layer_entity);

    for (const auto& [position, tile_id]: tile_cache) {
      const auto row = static_cast<usize>(position.row());
      const auto col = static_cast<usize>(position.col());
      layer.tiles[row][col] = tile_id;
    }
  }
}

void MapCommandCache::save_tiles(const Model& model,
                                 const Entity map_entity,
                                 const TilePos& begin,
                                 const TilePos& end)
{
  auto callback = [&, this](const Entity layer_entity, const TileLayer& layer) {
    auto& tile_cache = mCache[layer_entity];

    const auto end_row = end.row();
    const auto end_col = end.col();

    for (auto row = begin.row(); row < end_row; ++row) {
      for (auto col = begin.col(); col < end_col; ++col) {
        const TilePos position {row, col};

        const auto urow = static_cast<usize>(row);
        const auto ucol = static_cast<usize>(col);

        tile_cache.try_emplace(position, layer.tiles[urow][ucol]);
      }
    }
  };

  const auto& map = model.get<Map>(map_entity);
  sys::recurse_tile_layers(model, map.root_layer, callback);
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
