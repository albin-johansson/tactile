#include "map_command_cache.hpp"

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile {

void MapCommandCache::Clear() noexcept
{
  mCache.clear();
}

void MapCommandCache::RestoreTiles(entt::registry& registry)
{
  for (const auto& [layerId, tileCache] : mCache) {
    const auto entity = sys::FindLayer(registry, layerId);
    TACTILE_ASSERT(entity != entt::null);

    for (const auto& [position, tileId] : tileCache) {
      sys::SetTileInLayer(registry, entity, position, tileId);
    }
  }
}

void MapCommandCache::SaveTiles(const entt::registry& registry,
                                const map_position begin,
                                const map_position end)
{
  for (auto&& [entity, layer, tileLayer] : registry.view<Layer, TileLayer>().each()) {
    auto& tileCache = mCache[layer.id];

    const auto endRow = end.row();
    const auto endCol = end.col();
    for (auto row = begin.row(); row < endRow; ++row) {
      for (auto col = begin.col(); col < endCol; ++col) {
        const map_position position{row, col};
        const auto tile = sys::GetTileFromLayer(registry, entity, position);
        tileCache.try_emplace(position, tile);
      }
    }
  }
}

void MapCommandCache::MergeWith(const MapCommandCache& other)
{
  for (const auto& [otherLayer, otherTileCache] : other.mCache) {
    auto& tileCache = mCache[otherLayer];
    for (const auto& [otherPos, otherTile] : otherTileCache) {
      tileCache.try_emplace(otherPos, otherTile);
    }
  }
}

}  // namespace tactile
