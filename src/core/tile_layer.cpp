#include "tile_layer.h"

#include "map_position.h"

namespace tactile {

TileLayer::TileLayer(int nRows, int nCols) noexcept : m_visible{true}
{
  nRows = (nRows < 1) ? 1 : nRows;
  nCols = (nCols < 1) ? 1 : nCols;

  for (auto r = 0; r < nRows; ++r) {
    TileRow row;
    row.reserve(nCols);

    for (auto c = 0; c < nCols; ++c) {
      row.push_back(empty);
    }

    m_tiles.push_back(row);
  }
}

void TileLayer::flood(const MapPosition& position,
                      TileID target,
                      TileID replacement) noexcept
{
  // TODO implement flood fill algorithm
}

void TileLayer::add_row(TileID id) noexcept
{
  std::vector<TileID> row;
  row.reserve(cols());
  for (int i = 0; i < cols(); ++i) {
    row.push_back(id);
  }
  m_tiles.push_back(row);
}

void TileLayer::add_col(TileID id) noexcept
{
  for (auto& row : m_tiles) {
    row.push_back(id);
  }
}

void TileLayer::remove_row() noexcept
{
  if (m_tiles.size() > 1) {
    m_tiles.pop_back();
  }
}

void TileLayer::remove_col() noexcept
{
  for (auto& row : m_tiles) {
    if (row.size() > 1) {
      row.pop_back();
    }
  }
}

void TileLayer::set_tile(const MapPosition& position, TileID id) noexcept
{
  if (in_bounds(position)) {
    m_tiles.at(position.row()).at(position.col()) = id;
  }
}

void TileLayer::set_visible(bool visible) noexcept
{
  m_visible = visible;
}

int TileLayer::rows() const noexcept
{
  return static_cast<int>(m_tiles.size());
}

int TileLayer::cols() const noexcept
{
  return static_cast<int>(m_tiles.at(0).size());
}

Maybe<TileID> TileLayer::tile_at(const MapPosition& position) const noexcept
{
  if (in_bounds(position)) {
    return m_tiles.at(position.row()).at(position.col());
  } else {
    return nothing;
  }
}

bool TileLayer::in_bounds(const MapPosition& position) const noexcept
{
  return position.row() < m_tiles.size() &&
         position.col() < m_tiles.at(position.row()).size();
}

}  // namespace tactile
