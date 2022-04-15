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

#pragma once

#include <entt/entt.hpp>  // registry

#include "core/tile_pos.hpp"
#include "tactile.hpp"

namespace tactile {

class MapCommandCache final
{
  using tile_cache = TreeMap<TilePos, TileID>;
  using layer_cache = TreeMap<LayerID, tile_cache>;

 public:
  void clear() noexcept;

  void restore_tiles(entt::registry& registry);

  void save_tiles(const entt::registry& registry,
                  const TilePos& begin,
                  const TilePos& end);

  void merge_with(const MapCommandCache& other);

 private:
  layer_cache mCache;
};

}  // namespace tactile
