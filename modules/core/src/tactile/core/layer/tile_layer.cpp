// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include "tactile/core/layer/layer_visitor.hpp"

namespace tactile::core {

using int_literals::operator""_z;

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

auto TileLayer::is_valid_pos(const TilePos& pos) const -> bool
{
  const auto current_extent = extent();
  return pos.row >= 0_z &&                      //
         pos.col >= 0_z &&                      //
         pos.row < current_extent.row_count &&  //
         pos.col < current_extent.col_count;
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

auto TileLayer::meta() -> Metadata&
{
  return mDelegate.meta();
}

auto TileLayer::meta() const -> const Metadata&
{
  return mDelegate.meta();
}

}  // namespace tactile::core
