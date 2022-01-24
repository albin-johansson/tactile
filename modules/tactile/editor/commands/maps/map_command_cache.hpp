#pragma once

#include <functional>  // less
#include <map>         // map

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/map_position.hpp"

namespace tactile {

class MapCommandCache final {
  using TileCache = std::map<map_position, TileID, std::less<>>;
  using LayerCache = std::map<LayerID, TileCache, std::less<>>;

 public:
  void Clear() noexcept;

  void RestoreTiles(entt::registry& registry);

  void SaveTiles(const entt::registry& registry, map_position begin, map_position end);

  void MergeWith(const MapCommandCache& other);

 private:
  LayerCache mCache;
};

}  // namespace tactile
