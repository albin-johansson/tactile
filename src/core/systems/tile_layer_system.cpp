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

auto GetTileFromLayer(const entt::registry& registry,
                      const entt::entity entity,
                      const MapPosition& position) -> tile_id
{
  const auto& tileLayer = registry.get<TileLayer>(entity);
  const auto& matrix = tileLayer.matrix;

  const auto row = position.GetRow();
  const auto col = position.GetColumn();

  if (row < matrix.size() && col < matrix.at(0).size())
  {
    return matrix.at(row).at(col);
  }
  else
  {
    return empty_tile;
  }
}

}  // namespace Tactile::Sys
