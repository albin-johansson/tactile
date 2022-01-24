#include "tile_layer_system.hpp"

#include "assert.hpp"
#include "core/components/layer.hpp"

namespace tactile::sys {

void SetTileInLayer(entt::registry& registry,
                    const entt::entity entity,
                    const map_position& position,
                    const TileID tile)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

  auto& layer = registry.get<TileLayer>(entity);
  layer.matrix.at(position.row_index()).at(position.col_index()) = tile;
}

void SetTilesInLayer(entt::registry& registry,
                     const entt::entity entity,
                     const TileCache& tiles)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

  auto& matrix = registry.get<TileLayer>(entity).matrix;
  for (const auto& [position, tile] : tiles) {
    TACTILE_ASSERT(position.row_index() < matrix.size());
    TACTILE_ASSERT(position.col_index() < matrix.front().size());
    matrix[position.row_index()][position.col_index()] = tile;
  }
}

auto GetTileFromLayer(const entt::registry& registry,
                      const entt::entity entity,
                      const map_position& position) -> TileID
{
  const auto& tileLayer = registry.get<TileLayer>(entity);
  const auto& matrix = tileLayer.matrix;

  const auto row = position.row_index();
  const auto col = position.col_index();

  if (row < matrix.size() && col < matrix.at(0).size()) {
    return matrix.at(row).at(col);
  }
  else {
    return empty_tile;
  }
}

}  // namespace tactile::sys
