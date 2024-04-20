// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/dense_tile_matrix.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {
namespace {

void _add_rows(Vector<Vector<TileID>>& rows, const usize n, const usize cols)
{
  rows.reserve(rows.size() + n);
  for (usize i = 0; i < n; ++i) {
    rows.emplace_back(cols, kEmptyTile);
  }
}

void _add_columns(Vector<Vector<TileID>>& rows, const usize n)
{
  for (auto& row : rows) {
    row.reserve(row.size() + n);
    for (usize i = 0; i < n; ++i) {
      row.push_back(kEmptyTile);
    }
  }
}

void _remove_rows(Vector<Vector<TileID>>& rows, const usize n)
{
  for (usize i = 0; i < n; ++i) {
    TACTILE_ASSERT(!rows.empty());
    rows.pop_back();
  }
}

void _remove_columns(Vector<Vector<TileID>>& rows, const usize n)
{
  for (auto& row : rows) {
    for (usize i = 0; i < n; ++i) {
      TACTILE_ASSERT(!row.empty());
      row.pop_back();
    }
  }
}

}  // namespace

DenseTileMatrix::DenseTileMatrix(const MatrixExtent& extent)
  : mExtent {extent}
{
  mRows.reserve(mExtent.rows);
  mRows.assign(mExtent.rows, Vector<TileID>(mExtent.cols, kEmptyTile));
}

void DenseTileMatrix::resize(const MatrixExtent& new_extent)
{
  _set_column_count(new_extent.cols);
  _set_row_count(new_extent.rows);
}

void DenseTileMatrix::_set_row_count(const usize rows)
{
  if (rows != mExtent.rows) {
    if (rows > mExtent.rows) {
      _add_rows(mRows, rows - mExtent.rows, mExtent.cols);
    }
    else {
      _remove_rows(mRows, mExtent.rows - rows);
    }

    mExtent.rows = rows;
  }
}

void DenseTileMatrix::_set_column_count(const usize cols)
{
  if (cols != mExtent.cols) {
    if (cols > mExtent.cols) {
      _add_columns(mRows, cols - mExtent.cols);
    }
    else {
      _remove_columns(mRows, mExtent.cols - cols);
    }

    mExtent.cols = cols;
  }
}

auto DenseTileMatrix::at(const MatrixIndex index) -> TileID
{
  if (is_valid(index)) [[likely]] {
    return mRows[index.row][index.col];
  }

  throw Exception {"bad matrix index"};
}

auto DenseTileMatrix::at(const MatrixIndex index) const -> const TileID&
{
  if (is_valid(index)) [[likely]] {
    return mRows[index.row][index.col];
  }

  throw Exception {"bad matrix index"};
}

auto DenseTileMatrix::operator[](const MatrixIndex index) noexcept -> TileID&
{
  TACTILE_ASSERT_MSG(index.row < mExtent.rows, "bad row index");
  TACTILE_ASSERT_MSG(index.col < mExtent.cols, "bad column index");
  return mRows[index.row][index.col];
}

auto DenseTileMatrix::is_valid(const MatrixIndex& index) const noexcept -> bool
{
  return (index.row < mExtent.rows) && (index.col < mExtent.cols);
}

auto DenseTileMatrix::get_extent() const noexcept -> const MatrixExtent&
{
  return mExtent;
}

}  // namespace tactile
