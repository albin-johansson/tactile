#include "tile_layer.hpp"

#include <cassert>

#include "flood_fill.hpp"

namespace tactile {

tile_layer::tile_layer(int nRows, int nCols)
{
  nRows = (nRows < 1) ? 1 : nRows;
  nCols = (nCols < 1) ? 1 : nCols;

  for (auto r = 0; r < nRows; ++r) {
    tile_row row;
    row.reserve(static_cast<std::size_t>(nCols));

    for (auto c = 0; c < nCols; ++c) {
      row.push_back(empty);
    }

    m_tiles.push_back(row);
  }
}

void tile_layer::flood(const map_position& position,
                       tile_id target,
                       tile_id replacement) noexcept
{
  flood_fill(*this, position, target, replacement);
}

void tile_layer::add_row(tile_id id)
{
  const auto nCols = cols();

  tile_row row;
  row.reserve(static_cast<std::size_t>(nCols));
  for (int i = 0; i < nCols; ++i) {
    row.push_back(id);
  }
  m_tiles.push_back(row);
}

void tile_layer::add_col(tile_id id)
{
  for (auto& row : m_tiles) {
    row.push_back(id);
  }
}

void tile_layer::remove_row() noexcept
{
  if (m_tiles.size() > 1) {
    m_tiles.pop_back();
  }
}

void tile_layer::remove_col() noexcept
{
  for (auto& row : m_tiles) {
    if (row.size() > 1) {
      row.pop_back();
    }
  }
}

void tile_layer::set_tile(const map_position& position, tile_id id) noexcept
{
  if (in_bounds(position)) {
    const auto row = static_cast<std::size_t>(position.row());
    const auto col = static_cast<std::size_t>(position.col());
    m_tiles[row][col] = id;
  }
}

void tile_layer::set_visible(bool visible) noexcept
{
  m_visible = visible;
}

auto tile_layer::rows() const noexcept -> int
{
  return static_cast<int>(m_tiles.size());
}

auto tile_layer::cols() const noexcept -> int
{
  assert(!m_tiles.empty());
  return static_cast<int>(m_tiles[0].size());
}

auto tile_layer::tile_at(const map_position& position) const noexcept
    -> Maybe<tile_id>
{
  if (in_bounds(position)) {
    const auto row = static_cast<std::size_t>(position.row());
    const auto col = static_cast<std::size_t>(position.col());
    return m_tiles[row][col];
  } else {
    return std::nullopt;
  }
}

auto tile_layer::in_bounds(const map_position& position) const noexcept -> bool
{
  const auto row = static_cast<std::size_t>(position.row());
  const auto col = static_cast<std::size_t>(position.col());
  return (row < m_tiles.size()) && (col < m_tiles[row].size());
}

auto tile_layer::visible() const noexcept -> bool
{
  return m_visible;
}

}  // namespace tactile
