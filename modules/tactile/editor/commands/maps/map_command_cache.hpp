#pragma once

#include <functional>  // less
#include <map>         // map

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

namespace Tactile {

class MapCommandCache final
{
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
