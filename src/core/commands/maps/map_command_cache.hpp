#pragma once

#include <entt.hpp>    // registry
#include <functional>  // less
#include <map>         // map

#include "common/layer_id.hpp"
#include "common/tile_id.hpp"
#include "core/map_position.hpp"

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
