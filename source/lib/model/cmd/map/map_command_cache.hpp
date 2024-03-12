// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tile_pos.hpp"
#include "tactile/base/container/tree_map.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Map)

class MapCommandCache final {
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
