#include "layer.hpp"

#include <algorithm>  // clamp
#include <cassert>    // assert
#include <cmath>      // abs

#include "algorithm.hpp"
#include "flood_fill.hpp"
#include "tactile_error.hpp"

namespace tactile::core {
namespace {

[[nodiscard]] auto create_row(const col_t nCols, const tile_id value = empty)
    -> std::vector<tile_id>
{
  std::vector<tile_id> row;
  row.reserve(nCols.get());
  row.assign(nCols.get(), value);
  return row;
}

}  // namespace

layer::layer(const row_t nRows, const col_t nCols)
{
  if (nRows < 1_row || nCols < 1_col) {
    throw tactile_error{"Invalid layer dimensions!"};
  }

  m_tiles.reserve(nRows.get());
  m_tiles.assign(nRows.get(), create_row(nCols));

  assert(row_count() == nRows);
  assert(col_count() == nCols);
}

void layer::flood(const position& origin,
                  const tile_id replacement,
                  std::vector<position>& positions)
{
  flood_fill(*this, origin, replacement, positions);
}

void layer::remove_all(const tile_id id)
{
  const auto nRows = row_count().get();
  const auto nCols = col_count().get();
  for (auto r = 0; r < nRows; ++r) {
    for (auto c = 0; c < nCols; ++c) {
      if (m_tiles[r][c] == id) {
        m_tiles[r][c] = empty;
      }
    }
  }
}

void layer::add_row(const tile_id id)
{
  m_tiles.push_back(create_row(col_count(), id));
}

void layer::add_col(const tile_id id)
{
  for (auto& row : m_tiles) {
    row.push_back(id);
  }
}

void layer::remove_row() noexcept
{
  if (m_tiles.size() > 1) {
    m_tiles.pop_back();
  }
}

void layer::remove_col() noexcept
{
  for (auto& row : m_tiles) {
    if (row.size() > 1) {
      row.pop_back();
    }
  }
}

void layer::set_rows(const row_t nRows)
{
  assert(nRows >= 1_row);

  const auto current = row_count();

  if (nRows == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nRows.get());

  if (current < nRows) {
    invoke_n(diff, [this] {
      add_row(empty);
    });
  } else {
    invoke_n(diff, [this]() noexcept {
      remove_row();
    });
  }
}

void layer::set_cols(const col_t nCols)
{
  assert(nCols >= 1_col);

  const auto current = col_count();

  if (nCols == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nCols.get());

  if (current < nCols) {
    invoke_n(diff, [this] {
      add_col(empty);
    });
  } else {
    invoke_n(diff, [this]() noexcept {
      remove_col();
    });
  }
}

void layer::set_tile(const position& pos, const tile_id id) noexcept
{
  if (in_bounds(pos)) {
    m_tiles[pos.row_index()][pos.col_index()] = id;
  }
}

void layer::set_opacity(const double opacity)
{
  m_opacity = std::clamp(opacity, 0.0, 1.0);
}

void layer::set_name(QString name)
{
  m_name = std::move(name);
}

void layer::set_visible(const bool visible) noexcept
{
  m_visible = visible;
}

auto layer::row_count() const noexcept -> row_t
{
  return row_t{static_cast<int>(m_tiles.size())};
}

auto layer::col_count() const noexcept -> col_t
{
  assert(!m_tiles.empty());
  return col_t{static_cast<int>(m_tiles[0].size())};
}

auto layer::tile_count() const noexcept -> int
{
  return row_count().get() * col_count().get();
}

auto layer::tile_at(const position& pos) const -> maybe<tile_id>
{
  if (in_bounds(pos)) {
    return m_tiles[pos.row_index()][pos.col_index()];
  } else {
    return std::nullopt;
  }
}

auto layer::in_bounds(const position& pos) const noexcept -> bool
{
  const auto row = pos.row_index();
  return (row < m_tiles.size()) && (pos.col_index() < m_tiles[row].size());
}

}  // namespace tactile::core
