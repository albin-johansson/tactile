// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "abstract_layer.hpp"

namespace tactile {

void AbstractLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void AbstractLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

void AbstractLayer::set_parent(const Maybe<UUID>& parent_id)
{
  mDelegate.set_parent(parent_id);
}

void AbstractLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

auto AbstractLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto AbstractLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto AbstractLayer::get_ctx() -> ContextInfo&
{
  return mDelegate.get_ctx();
}

auto AbstractLayer::get_ctx() const -> const ContextInfo&
{
  return mDelegate.get_ctx();
}

auto AbstractLayer::get_uuid() const -> const UUID&
{
  return mDelegate.get_ctx().get_uuid();
}

auto AbstractLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto AbstractLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

}  // namespace tactile
