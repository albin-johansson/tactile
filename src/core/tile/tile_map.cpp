#include "tile_map.h"

#include "tile.h"

namespace tactile {
namespace {

[[nodiscard]] int clamp_map_dimension(int dim) noexcept
{
  return (dim < 1) ? 1 : dim;
}

}  // namespace

TileMap::TileMap(int nRows, int nCols)
    : m_nRows{clamp_map_dimension(nRows)}, m_nCols{clamp_map_dimension(nCols)}
{
  m_layers.emplace_back(nRows, nCols);
}

void TileMap::add_row(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
}

void TileMap::add_col(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
}

void TileMap::set_rows(int nRows) noexcept
{
  m_nRows = clamp_map_dimension(nRows);
}

void TileMap::set_cols(int nCols) noexcept
{
  m_nCols = clamp_map_dimension(nCols);
}

int TileMap::rows() const noexcept
{
  return m_nRows;
}

int TileMap::cols() const noexcept
{
  return m_nCols;
}

int TileMap::get_layer_amount() const noexcept
{
  return static_cast<int>(m_layers.size());
}

}  // namespace tactile
