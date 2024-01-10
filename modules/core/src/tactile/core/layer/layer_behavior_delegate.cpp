// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/layer_behavior_delegate.hpp"

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

auto LayerBehaviorDelegate::meta() -> Metadata&
{
  return mMeta;
}

auto LayerBehaviorDelegate::meta() const -> const Metadata&
{
  return mMeta;
}

auto LayerBehaviorDelegate::clone() const -> LayerBehaviorDelegate
{
  auto other = LayerBehaviorDelegate {};

  // The UUID and persistent ID attributes are intentionally ignored.
  other.mMeta = mMeta.clone();
  other.mOpacity = mOpacity;
  other.mVisible = mVisible;

  return other;
}

}  // namespace tactile
