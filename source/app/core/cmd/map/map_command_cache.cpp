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

#include "core/layer/layer_visitor.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"

namespace tactile {

struct SaveTilesVisitor final : IConstLayerVisitor
{
  MapCommandCache* self {};
  TilePos          begin;
  TilePos          end;

  void visit(const TileLayer& layer) override
  {
    auto& tileCache = self->mCache[layer.get_uuid()];

    const auto endRow = end.row();
    const auto endCol = end.col();
    for (auto row = begin.row(); row < endRow; ++row) {
      for (auto col = begin.col(); col < endCol; ++col) {
        const TilePos position {row, col};
        const auto    tile = layer.tile_at(position);
        tileCache.try_emplace(position, tile);
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
  for (const auto& [layerId, tileCache] : mCache) {
    auto& layer = map.view_tile_layer(layerId);

    for (const auto& [position, tileId] : tileCache) {
      layer.set_tile(position, tileId);
    }
  }
}

void MapCommandCache::save_tiles(const Map& map, const TilePos& begin, const TilePos& end)
{
  SaveTilesVisitor visitor;
  visitor.self = this;
  visitor.begin = begin;
  visitor.end = end;
  map.visit_layers(visitor);
}

void MapCommandCache::merge_with(const MapCommandCache& other)
{
  for (const auto& [otherLayer, otherTileCache] : other.mCache) {
    auto& tileCache = mCache[otherLayer];
    for (const auto& [otherPos, otherTile] : otherTileCache) {
      tileCache.try_emplace(otherPos, otherTile);
    }
  }
}

}  // namespace tactile
