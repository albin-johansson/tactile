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

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/tile_pos.hpp"

namespace tactile {

class MapCommandCache final
{
  friend struct SaveTilesVisitor;

  using TileCache = TreeMap<TilePos, TileID>;
  using LayerCache = TreeMap<UUID, TileCache>;

 public:
  void clear() noexcept;

  void restore_tiles(Map& map);

  void save_tiles(const Map& map, const TilePos& begin, const TilePos& end);

  void merge_with(const MapCommandCache& other);

 private:
  LayerCache mCache;
};

}  // namespace tactile
