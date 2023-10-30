// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/layer_behavior_delegate.hpp"

#include <algorithm>  // clamp

namespace tactile {

void LayerBehaviorDelegate::set_persistent_id(const Maybe<int32> id)
{
  mPersistentId = id;
}

void LayerBehaviorDelegate::set_opacity(const float opacity)
{
  mOpacity = std::clamp(opacity, 0.0f, 1.0f);
}

void LayerBehaviorDelegate::set_visible(const bool visible)
{
  mVisible = visible;
}

auto LayerBehaviorDelegate::get_persistent_id() const -> Maybe<int32>
{
  return mPersistentId;
}

auto LayerBehaviorDelegate::get_opacity() const -> float
{
  return mOpacity;
}

auto LayerBehaviorDelegate::is_visible() const -> bool
{
  return mVisible;
}

auto LayerBehaviorDelegate::get_meta() -> Metadata&
{
  return mMeta;
}

auto LayerBehaviorDelegate::get_meta() const -> const Metadata&
{
  return mMeta;
}

}  // namespace tactile
