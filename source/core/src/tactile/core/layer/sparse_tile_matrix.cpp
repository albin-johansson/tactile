// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/sparse_tile_matrix.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

SparseTileMatrix::SparseTileMatrix(const MatrixExtent& extent)
  : mExtent {extent}
{}

void SparseTileMatrix::resize(const MatrixExtent& new_extent)
{
  mExtent = new_extent;
  std::erase_if(mTiles,
                [this](const auto& pair) { return !is_valid(pair.first); });
}

auto SparseTileMatrix::at(const MatrixIndex index) -> TileID
{
  if (is_valid(index)) [[unlikely]] {
    throw Exception {"bad matrix index"};
  }

  if (const auto* tile_id = find_in(mTiles, index)) {
    return *tile_id;
  }

  return kEmptyTile;
}

auto SparseTileMatrix::at(const MatrixIndex index) const -> TileID
{
  if (is_valid(index)) [[unlikely]] {
    throw Exception {"bad matrix index"};
  }

  if (const auto* tile_id = find_in(mTiles, index)) {
    return *tile_id;
  }

  return kEmptyTile;
}

auto SparseTileMatrix::operator[](const MatrixIndex index) -> TileID&
{
  TACTILE_ASSERT(is_valid(index));
  return mTiles[index];
}

auto SparseTileMatrix::is_valid(const MatrixIndex& index) const noexcept -> bool
{
  return (index.row >= 0) &&            //
         (index.col >= 0) &&            //
         (index.row < mExtent.rows) &&  //
         (index.col < mExtent.cols);
}

auto SparseTileMatrix::get_extent() const noexcept -> const MatrixExtent&
{
  return mExtent;
}

}  // namespace tactile
