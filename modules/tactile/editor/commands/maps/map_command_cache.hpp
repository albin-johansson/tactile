#pragma once

#include <functional>  // less
#include <map>         // map

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/map_position.hpp"

namespace Tactile {

class MapCommandCache final {
  using TileCache = std::map<MapPosition, TileID, std::less<>>;
  using LayerCache = std::map<LayerID, TileCache, std::less<>>;

 public:
  void Clear() noexcept;

  void RestoreTiles(entt::registry& registry);

  void SaveTiles(const entt::registry& registry, MapPosition begin, MapPosition end);

  void MergeWith(const MapCommandCache& other);

 private:
  LayerCache mCache;
};

}  // namespace Tactile
