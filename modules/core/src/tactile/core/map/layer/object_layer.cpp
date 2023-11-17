// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/object_layer.hpp"

#include "tactile/core/map/layer/layer_visitor.hpp"

namespace tactile {

void ObjectLayer::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::set_persistent_id(const Maybe<int32> id)
{
  mDelegate.set_persistent_id(id);
}

void ObjectLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void ObjectLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto ObjectLayer::get_persistent_id() const -> Maybe<int32>
{
  return mDelegate.get_persistent_id();
}

auto ObjectLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto ObjectLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto ObjectLayer::clone() const -> Shared<ILayer>
{
  auto clone = make_shared<ObjectLayer>(*this);

  clone->set_persistent_id(kNone);

  return clone;
}

auto ObjectLayer::get_meta() -> Metadata&
{
  return mDelegate.get_meta();
}

auto ObjectLayer::get_meta() const -> const Metadata&
{
  return mDelegate.get_meta();
}

}  // namespace tactile
