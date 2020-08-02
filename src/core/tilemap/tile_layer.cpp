#include "tile_layer.hpp"

#include "flood_fill.hpp"

namespace tactile {

TileLayer::TileLayer(int nRows, int nCols) noexcept : m_visible{true}
{
  nRows = (nRows < 1) ? 1 : nRows;
  nCols = (nCols < 1) ? 1 : nCols;

  for (auto r = 0; r < nRows; ++r) {
    TileRow row;
    row.reserve(static_cast<std::size_t>(nCols));

    for (auto c = 0; c < nCols; ++c) {
      row.push_back(empty);
    }

    m_tiles.push_back(row);
  }
}

void TileLayer::flood(const map_position& position,
                      tile_id target,
                      tile_id replacement) noexcept
{
  flood_fill(*this, position, target, replacement);
}

void TileLayer::add_row(tile_id id) noexcept
{
  const auto nCols = cols();

  TileRow row;
  row.reserve(static_cast<std::size_t>(nCols));
  for (int i = 0; i < nCols; ++i) {
    row.push_back(id);
  }
  m_tiles.push_back(row);
}

void TileLayer::add_col(tile_id id) noexcept
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

void TileLayer::set_tile(const map_position& position, tile_id id) noexcept
{
  if (in_bounds(position)) {
    const auto row = static_cast<std::size_t>(position.row());
    const auto col = static_cast<std::size_t>(position.col());
    m_tiles.at(row).at(col) = id;
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

Maybe<tile_id> TileLayer::tile_at(const map_position& position) const noexcept
{
  if (in_bounds(position)) {
    const auto row = static_cast<std::size_t>(position.row());
    const auto col = static_cast<std::size_t>(position.col());
    return m_tiles.at(row).at(col);
  } else {
    return nothing;
  }
}

bool TileLayer::in_bounds(const map_position& position) const noexcept
{
  const auto row = static_cast<std::size_t>(position.row());
  const auto col = static_cast<std::size_t>(position.col());
  return row < m_tiles.size() && col < m_tiles.at(row).size();
}

}  // namespace tactile
