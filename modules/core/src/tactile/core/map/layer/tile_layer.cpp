// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/tile_layer.hpp"

#include "tactile/core/map/layer/layer_visitor.hpp"
#include "tactile/foundation/container/queue.hpp"
#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/functional/utility.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

using namespace tactile::int_literals;

namespace tactile {

TileLayer::TileLayer(const ssize row_count, const ssize col_count)
  : mExtent {(row_count > 0) ? row_count : throw Exception {"invalid row count"},
             (col_count > 0) ? col_count : throw Exception {"invalid column count"}},
    mTileMatrix {make_tile_matrix(mExtent)}
{}

TileLayer::TileLayer(const MatrixExtent extent)
  : TileLayer {extent.row_count, extent.col_count}
{}

void TileLayer::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void TileLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void TileLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

void TileLayer::resize(const ssize row_count, const ssize col_count)
{
  if (row_count < 1) {
    throw Exception {"Invalid row count"};
  }

  if (col_count < 1) {
    throw Exception {"Invalid column count"};
  }

  if (row_count < mExtent.row_count) {
    repeat(mExtent.row_count - row_count, [this] { _remove_row(); });
  }
  else {
    repeat(row_count - mExtent.row_count, [this] { _add_row(); });
  }

  if (col_count < mExtent.col_count) {
    repeat(mExtent.col_count - col_count, [this] { _remove_column(); });
  }
  else {
    repeat(col_count - mExtent.col_count, [this] { _add_column(); });
  }

  TACTILE_ASSERT(mExtent.row_count == row_count);
  TACTILE_ASSERT(mExtent.col_count == col_count);
}

void TileLayer::_add_row()
{
  ++mExtent.row_count;
  mTileMatrix.push_back(make_tile_row(mExtent.col_count));
}

void TileLayer::_add_column()
{
  ++mExtent.col_count;
  for (auto& tile_row : mTileMatrix) {
    tile_row.push_back(kEmptyTile);
  }
}

void TileLayer::_remove_row()
{
  TACTILE_ASSERT(mExtent.row_count > 1);

  --mExtent.row_count;
  mTileMatrix.pop_back();
}

void TileLayer::_remove_column()
{
  TACTILE_ASSERT(mExtent.col_count > 1);

  --mExtent.col_count;
  for (auto& tile_row : mTileMatrix) {
    tile_row.pop_back();
  }
}

void TileLayer::flood(const TilePos& start_pos,
                      const TileID new_id,
                      Vector<TilePos>* affected_positions)
{
  if (!is_valid_pos(start_pos) || get_tile(start_pos) == new_id) {
    return;
  }

  const auto target_id = get_tile(start_pos);

  Queue<TilePos> remaining_positions;

  auto maybe_update_tile_at = [&, this](const TilePos& tile_pos) {
    if (get_tile(tile_pos) == target_id) {
      set_tile(tile_pos, new_id);

      if (affected_positions != nullptr) {
        affected_positions->push_back(tile_pos);
      }

      remaining_positions.push(tile_pos);
    }
  };

  maybe_update_tile_at(start_pos);

  while (!remaining_positions.empty()) {
    const auto current_position = remaining_positions.front();
    remaining_positions.pop();

    maybe_update_tile_at(current_position - TilePos {0_z, 1_z});
    maybe_update_tile_at(current_position + TilePos {0_z, 1_z});
    maybe_update_tile_at(current_position - TilePos {1_z, 0_z});
    maybe_update_tile_at(current_position + TilePos {1_z, 0_z});
  }
}

void TileLayer::set_tile(const TilePos& pos, const TileID id)
{
  if (is_valid_pos(pos)) [[likely]] {
    const auto u_row = as_unsigned(pos.row);
    const auto u_col = as_unsigned(pos.col);

    mTileMatrix[u_row][u_col] = id;
  }
}

auto TileLayer::get_tile(const TilePos& pos) const -> Maybe<TileID>
{
  if (is_valid_pos(pos)) [[likely]] {
    const auto u_row = as_unsigned(pos.row);
    const auto u_col = as_unsigned(pos.col);

    return mTileMatrix[u_row][u_col];
  }

  return kNone;
}

auto TileLayer::is_valid_pos(const TilePos& pos) const -> bool
{
  return pos.row >= 0_z &&               //
         pos.col >= 0_z &&               //
         pos.row < mExtent.row_count &&  //
         pos.col < mExtent.col_count;
}

auto TileLayer::row_count() const -> ssize
{
  return mExtent.row_count;
}

auto TileLayer::column_count() const -> ssize
{
  return mExtent.col_count;
}

void TileLayer::set_persistent_id(const Maybe<int32> id)
{
  mDelegate.set_persistent_id(id);
}

void TileLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void TileLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto TileLayer::get_persistent_id() const -> Maybe<int32>
{
  return mDelegate.get_persistent_id();
}

auto TileLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto TileLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto TileLayer::clone() const -> Shared<ILayer>
{
  auto other = make_shared<TileLayer>(mExtent);

  // The persistent ID attribute is intentionally ignored.
  other->mDelegate = mDelegate.clone();
  other->mTileMatrix = mTileMatrix;

  return other;
}

auto TileLayer::meta() -> Metadata&
{
  return mDelegate.meta();
}

auto TileLayer::meta() const -> const Metadata&
{
  return mDelegate.meta();
}

}  // namespace tactile
