// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_matrix.hpp"

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

auto operator<<(std::ostream& stream, const MatrixIndex& index) -> std::ostream&
{
  stream << '(' << index.row << ';' << index.col << ')';
  return stream;
}

auto operator<<(std::ostream& stream, const MatrixExtent& extent) -> std::ostream&
{
  stream << '(' << extent.rows << ';' << extent.cols << ')';
  return stream;
}

TileMatrix::TileMatrix(const MatrixExtent& extent)
  : mExtent {extent}
{
  mRows.reserve(mExtent.rows);
  mRows.assign(mExtent.rows, Vector<TileID>(mExtent.cols, kEmptyTile));
}

void TileMatrix::resize(const MatrixExtent& new_extent)
{
  set_column_count(new_extent.cols);
  set_row_count(new_extent.rows);
}

void TileMatrix::set_row_count(const usize rows)
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

void TileMatrix::set_column_count(const usize cols)
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

auto TileMatrix::at(const MatrixIndex index) -> TileID&
{
  if (is_valid(index)) [[likely]] {
    return operator[](index);
  }

  throw Exception {"bad matrix index"};
}

auto TileMatrix::at(const MatrixIndex index) const -> TileID
{
  if (is_valid(index)) [[likely]] {
    return operator[](index);
  }

  throw Exception {"bad matrix index"};
}

auto TileMatrix::operator[](const MatrixIndex index) noexcept -> TileID&
{
  TACTILE_ASSERT_MSG(index.row < mExtent.rows, "bad row index");
  TACTILE_ASSERT_MSG(index.col < mExtent.cols, "bad column index");
  return mRows[index.row][index.col];
}

auto TileMatrix::operator[](const MatrixIndex index) const noexcept -> TileID
{
  TACTILE_ASSERT_MSG(index.row < mExtent.rows, "bad row index");
  TACTILE_ASSERT_MSG(index.col < mExtent.cols, "bad column index");
  return mRows[index.row][index.col];
}

auto TileMatrix::is_valid(const MatrixIndex& index) const noexcept -> bool
{
  return (index.row < mExtent.rows) && (index.col < mExtent.cols);
}

auto TileMatrix::get_extent() const noexcept -> const MatrixExtent&
{
  return mExtent;
}

}  // namespace tactile
