#include "tile_layer_system.hpp"

#include "core/components/tile_layer.hpp"

namespace Tactile::Sys {

auto MakeTileRow(const col_t nCols, const tile_id value) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

auto MakeTileMatrix(const row_t nRows, const col_t nCols) -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols, empty_tile));
  return tiles;
}

auto GetTile(const entt::registry& registry,
             const entt::entity entity,
             const row_t row,
             const col_t col) -> Maybe<tile_id>
{
  const auto& tileLayer = registry.get<TileLayer>(entity);
  const auto& matrix = tileLayer.matrix;

  if (row < matrix.size() && col < matrix.at(0).size())
  {
    return matrix.at(row).at(col);
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile::Sys
