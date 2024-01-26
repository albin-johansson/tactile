// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/sparse_tile_layer.hpp"

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile {

SparseTileLayer::SparseTileLayer(const ssize row_count, const ssize col_count)
  : mExtent {(row_count > 0) ? row_count : throw Exception {"invalid row count"},
             (col_count > 0) ? col_count : throw Exception {"invalid column count"}}
{}

SparseTileLayer::SparseTileLayer(const MatrixExtent extent)
  : SparseTileLayer {extent.row_count, extent.col_count}
{}

auto SparseTileLayer::set_tile(const TilePos& pos, const TileID id) -> Result<void>
{
  if (is_valid_pos(pos)) {
    if (id == kEmptyTile) {
      mTiles.erase(pos);
    }
    else {
      mTiles.insert_or_assign(pos, id);
    }

    return kOK;
  }

  return unexpected(make_error(GenericError::kInvalidArgument));
}

auto SparseTileLayer::tile_at(const TilePos& pos) const -> Maybe<TileID>
{
  if (is_valid_pos(pos)) {
    const auto tile_iter = mTiles.find(pos);
    return (tile_iter != mTiles.end()) ? tile_iter->second : kEmptyTile;
  }

  return kNothing;
}

auto SparseTileLayer::set_extent(const MatrixExtent& extent) -> Result<void>
{
  if (extent.row_count <= 0 || extent.col_count <= 0) {
    return unexpected(make_generic_error(GenericError::kInvalidArgument));
  }

  mExtent = extent;

  std::erase_if(mTiles, [&](const auto& pair) { return !is_valid_pos(pair.first); });

  return kOK;
}

auto SparseTileLayer::extent() const -> MatrixExtent
{
  return mExtent;
}

auto SparseTileLayer::clone() const -> Shared<ILayer>
{
  auto other = make_shared<SparseTileLayer>(mExtent);

  // The persistent ID attribute is intentionally ignored.
  other->mDelegate = mDelegate.clone();
  other->mTiles = mTiles;

  return other;
}

}  // namespace tactile
