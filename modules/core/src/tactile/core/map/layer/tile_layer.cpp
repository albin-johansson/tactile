// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/tile_layer.hpp"

#include "tactile/core/container/queue.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/error.hpp"
#include "tactile/core/functional/utility.hpp"
#include "tactile/core/map/layer/layer_visitor.hpp"

using namespace tactile::int_literals;

namespace tactile {

TileLayer::TileLayer(const usize row_count, const usize col_count)
  : mRowCount {(row_count > 0) ? row_count : throw Error {"Invalid row count"}},
    mColCount {(col_count > 0) ? col_count : throw Error {"Invalid column count"}},
    mTileMatrix {make_tile_matrix(row_count, col_count)}
{}

void TileLayer::resize(const usize row_count, const usize col_count)
{
  if (row_count < 1) {
    throw Error {"Invalid row count"};
  }

  if (col_count < 1) {
    throw Error {"Invalid column count"};
  }

  if (row_count < mRowCount) {
    repeat(mRowCount - row_count, [this] { _remove_row(); });
  }
  else {
    repeat(row_count - mRowCount, [this] { _add_row(); });
  }

  if (col_count < mColCount) {
    repeat(mColCount - col_count, [this] { _remove_column(); });
  }
  else {
    repeat(col_count - mColCount, [this] { _add_column(); });
  }

  TACTILE_ASSERT(mRowCount == row_count);
  TACTILE_ASSERT(mColCount == col_count);
}

void TileLayer::_add_row()
{
  ++mRowCount;
  mTileMatrix.push_back(make_tile_row(mColCount));
}

void TileLayer::_add_column()
{
  ++mColCount;
  for (auto& tile_row : mTileMatrix) {
    tile_row.push_back(kEmptyTile);
  }
}

void TileLayer::_remove_row()
{
  TACTILE_ASSERT(mRowCount > 1);

  --mRowCount;
  mTileMatrix.pop_back();
}

void TileLayer::_remove_column()
{
  TACTILE_ASSERT(mColCount > 1);

  --mColCount;
  for (auto& tile_row : mTileMatrix) {
    tile_row.pop_back();
  }
}

void TileLayer::flood(const TilePos& start_pos,
                      const TileID new_id,
                      Vector<TilePos>* affected_positions)
{
  if (!is_valid_pos(start_pos)) {
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
    const auto u_row = static_cast<usize>(pos.row);
    const auto u_col = static_cast<usize>(pos.col);

    mTileMatrix[u_row][u_col] = id;
  }
}

auto TileLayer::get_tile(const TilePos& pos) const -> Maybe<TileID>
{
  if (is_valid_pos(pos)) [[likely]] {
    const auto u_row = static_cast<usize>(pos.row);
    const auto u_col = static_cast<usize>(pos.col);

    return mTileMatrix[u_row][u_col];
  }

  return kNone;
}

auto TileLayer::is_valid_pos(const TilePos& pos) const -> bool
{
  if (pos.row >= 0_z && pos.col >= 0_z) {
    const auto u_row = static_cast<usize>(pos.row);
    const auto u_col = static_cast<usize>(pos.col);

    return u_row < mRowCount && u_col < mColCount;
  }

  return false;
}

auto TileLayer::row_count() const -> usize
{
  return mRowCount;
}

auto TileLayer::column_count() const -> usize
{
  return mColCount;
}

void TileLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
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
  auto clone = make_shared<TileLayer>(*this);

  clone->set_persistent_id(kNone);

  return clone;
}

auto TileLayer::get_meta() -> Metadata&
{
  return mDelegate.get_meta();
}

auto TileLayer::get_meta() const -> const Metadata&
{
  return mDelegate.get_meta();
}

}  // namespace tactile
