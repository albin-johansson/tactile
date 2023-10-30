// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/group_layer.hpp"

#include "tactile/core/map/layer/layer_visitor.hpp"

namespace tactile {

void GroupLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void GroupLayer::set_persistent_id(const Maybe<int32> id)
{
  mDelegate.set_persistent_id(id);
}

void GroupLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void GroupLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto GroupLayer::get_persistent_id() const -> Maybe<int32>
{
  return mDelegate.get_persistent_id();
}

auto GroupLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto GroupLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto GroupLayer::clone() const -> Shared<ILayer>
{
  auto clone = make_shared<GroupLayer>(*this);

  clone->set_persistent_id(kNone);

  return clone;
}

auto GroupLayer::get_meta() -> Metadata&
{
  return mDelegate.get_meta();
}

auto GroupLayer::get_meta() const -> const Metadata&
{
  return mDelegate.get_meta();
}

}  // namespace tactile
