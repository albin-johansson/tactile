#include "tile_layer_system.hpp"

#include "assert.hpp"
#include "core/components/layer.hpp"

namespace tactile::Sys {

void SetTileInLayer(entt::registry& registry,
                    const entt::entity entity,
                    const MapPosition& position,
                    const TileID tile)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

  auto& layer = registry.get<TileLayer>(entity);
  layer.matrix.at(position.GetRowIndex()).at(position.GetColumnIndex()) = tile;
}

void SetTilesInLayer(entt::registry& registry,
                     const entt::entity entity,
                     const TileCache& tiles)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

  auto& matrix = registry.get<TileLayer>(entity).matrix;
  for (const auto& [position, tile] : tiles) {
    TACTILE_ASSERT(position.GetRowIndex() < matrix.size());
    TACTILE_ASSERT(position.GetColumnIndex() < matrix.front().size());
    matrix[position.GetRowIndex()][position.GetColumnIndex()] = tile;
  }
}

auto GetTileFromLayer(const entt::registry& registry,
                      const entt::entity entity,
                      const MapPosition& position) -> TileID
{
  const auto& tileLayer = registry.get<TileLayer>(entity);
  const auto& matrix = tileLayer.matrix;

  const auto row = position.GetRowIndex();
  const auto col = position.GetColumnIndex();

  if (row < matrix.size() && col < matrix.at(0).size()) {
    return matrix.at(row).at(col);
  }
  else {
    return empty_tile;
  }
}

}  // namespace tactile::Sys
