#include "tile_map.h"

#include "tile.h"

namespace tactile {
namespace {

[[nodiscard]] int clamp_map_dimension(int dim) noexcept
{
  return (dim < 1) ? 1 : dim;
}

}  // namespace

TileMap::TileMap(int nRows_, int nCols_)
    : nRows{clamp_map_dimension(nRows_)}, nCols{clamp_map_dimension(nCols_)}
{
  layers.emplace_back(nRows, nCols);
}

void TileMap::add_row(TileID id) noexcept
{
  for (auto& layer : layers) {
    layer.add_row(id, nCols);
  }
}

void TileMap::add_col(TileID id) noexcept
{
  for (auto& layer : layers) {
    layer.add_col(id);
  }
}

void TileMap::set_rows(int nRows_) noexcept
{
  nRows = clamp_map_dimension(nRows_);
}

void TileMap::set_cols(int nCols_) noexcept
{
  nCols = clamp_map_dimension(nCols_);
}

int TileMap::rows() const noexcept
{
  return nRows;
}

int TileMap::cols() const noexcept
{
  return nCols;
}

int TileMap::get_layer_amount() const noexcept
{
  return static_cast<int>(layers.size());
}

}  // namespace tactile
