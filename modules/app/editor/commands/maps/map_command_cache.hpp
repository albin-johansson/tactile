#pragma once

#include <functional>  // less
#include <map>         // map

#include <entt/entt.hpp>  // registry

#include "core/tile_position.hpp"
#include "tactile_def.hpp"

namespace tactile {

class MapCommandCache final {
  using TileCache = std::map<tile_position, TileID, std::less<>>;
  using LayerCache = std::map<LayerID, TileCache, std::less<>>;

 public:
  void Clear() noexcept;

  void RestoreTiles(entt::registry& registry);

  void SaveTiles(const entt::registry& registry, tile_position begin, tile_position end);

  void MergeWith(const MapCommandCache& other);

 private:
  LayerCache mCache;
};

}  // namespace tactile
