// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/tile_layer.hpp"

#include "tactile/core/map/layer/layer_visitor.hpp"

namespace tactile {

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
