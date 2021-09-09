#include "map_command_cache.hpp"

#include <cassert>  // assert

#include "core/components/tile_layer.hpp"
#include "core/systems/layer_system.hpp"
#include "core/systems/tile_layer_system.hpp"

namespace Tactile {

void MapCommandCache::Clear() noexcept
{
  mCache.clear();
}

void MapCommandCache::RestoreTiles(entt::registry& registry)
{
  for (const auto& [layerId, tileCache] : mCache)
  {
    const auto entity = Sys::FindLayer(registry, layerId);
    assert(entity != entt::null);

    for (const auto& [position, tileId] : tileCache)
    {
      Sys::SetTileInLayer(registry, entity, position, tileId);
    }
  }
}

void MapCommandCache::SaveTiles(const entt::registry& registry,
                                const MapPosition begin,
                                const MapPosition end)
{
  for (auto&& [entity, layer, tileLayer] : registry.view<Layer, TileLayer>().each())
  {
    auto& tileCache = mCache[layer.id];

    const auto endRow = end.GetRow();
    const auto endCol = end.GetColumn();
    for (auto row = begin.GetRow(); row < endRow; ++row)
    {
      for (auto col = begin.GetColumn(); col < endCol; ++col)
      {
        const MapPosition position{row, col};
        const auto tile = Sys::GetTileFromLayer(registry, entity, position);
        tileCache.try_emplace(position, tile);
      }
    }
  }
}

void MapCommandCache::MergeWith(const MapCommandCache& other)
{
  for (const auto& [otherLayer, otherTileCache] : other.mCache)
  {
    auto& tileCache = mCache[otherLayer];
    for (const auto& [otherPos, otherTile] : otherTileCache)
    {
      tileCache.try_emplace(otherPos, otherTile);
    }
  }
}

}  // namespace Tactile
