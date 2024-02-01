// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/dense_tile_layer.hpp"

#include <cmath>  // abs

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/functional/utility.hpp"

namespace tactile::core {

DenseTileLayer::DenseTileLayer(const ssize row_count, const ssize col_count)
  : mExtent {(row_count > 0) ? row_count : throw Exception {"invalid row count"},
             (col_count > 0) ? col_count : throw Exception {"invalid column count"}},
    mTileMatrix {make_tile_matrix(row_count, col_count)}
{}

DenseTileLayer::DenseTileLayer(const MatrixExtent extent)
  : DenseTileLayer {extent.row_count, extent.col_count}
{}

auto DenseTileLayer::set_tile(const TilePos& pos, const TileID id) -> Result<void>
{
  if (is_valid_pos(pos)) [[likely]] {
    const auto u_row = static_cast<usize>(pos.row);
    const auto u_col = static_cast<usize>(pos.col);

    mTileMatrix[u_row][u_col] = id;
    return kOK;
  }

  return unexpected(make_error(GenericError::kInvalidArgument));
}

auto DenseTileLayer::tile_at(const TilePos& pos) const -> Maybe<TileID>
{
  if (is_valid_pos(pos)) [[likely]] {
    const auto u_row = static_cast<usize>(pos.row);
    const auto u_col = static_cast<usize>(pos.col);

    return mTileMatrix[u_row][u_col];
  }

  return kNothing;
}

auto DenseTileLayer::set_extent(const MatrixExtent& extent) -> Result<void>
{
  if (mExtent == extent) {
    return kOK;
  }

  if (extent.row_count <= 0 || extent.col_count <= 0) {
    return unexpected(make_generic_error(GenericError::kInvalidArgument));
  }

  const auto row_delta = extent.row_count - mExtent.row_count;
  const auto col_delta = extent.col_count - mExtent.col_count;

  if (col_delta != 0) {
    if (col_delta > 0) {
      repeat(col_delta, [this] { _add_column(); });
    }
    else {
      repeat(std::abs(col_delta), [this] { _remove_column(); });
    }
  }

  if (row_delta != 0) {
    if (row_delta > 0) {
      repeat(row_delta, [this] { _add_row(); });
    }
    else {
      repeat(std::abs(row_delta), [this] { _remove_row(); });
    }
  }

  mExtent = extent;
  return kOK;
}

auto DenseTileLayer::extent() const -> MatrixExtent
{
  return mExtent;
}

auto DenseTileLayer::clone() const -> Shared<ILayer>
{
  auto other = make_shared<DenseTileLayer>(mExtent);

  // The persistent ID attribute is intentionally ignored.
  other->mDelegate = mDelegate.clone();
  other->mTileMatrix = mTileMatrix;

  return other;
}

void DenseTileLayer::_add_row()
{
  ++mExtent.row_count;
  mTileMatrix.push_back(make_tile_row(mExtent.col_count));
}

void DenseTileLayer::_add_column()
{
  ++mExtent.col_count;
  for (auto& tile_row : mTileMatrix) {
    tile_row.push_back(kEmptyTile);
  }
}

void DenseTileLayer::_remove_row()
{
  TACTILE_ASSERT(mExtent.row_count > 1);

  --mExtent.row_count;
  mTileMatrix.pop_back();
}

void DenseTileLayer::_remove_column()
{
  TACTILE_ASSERT(mExtent.col_count > 1);

  --mExtent.col_count;
  for (auto& tile_row : mTileMatrix) {
    tile_row.pop_back();
  }
}

}  // namespace tactile::core