// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "layer_delegate.hpp"

#include <algorithm>  // clamp

namespace tactile {

void LayerDelegate::set_opacity(const float opacity)
{
  mOpacity = std::clamp(opacity, 0.0f, 1.0f);
}

void LayerDelegate::set_visible(const bool visible)
{
  mVisible = visible;
}

void LayerDelegate::set_parent(const Maybe<UUID>& id)
{
  mParentId = id;
}

void LayerDelegate::set_meta_id(const int32 id)
{
  mMetaId = id;
}

auto LayerDelegate::get_opacity() const -> float
{
  return mOpacity;
}

auto LayerDelegate::is_visible() const -> bool
{
  return mVisible;
}

auto LayerDelegate::get_parent() const -> const Maybe<UUID>&
{
  return mParentId;
}

auto LayerDelegate::get_meta_id() const -> const Maybe<int32>&
{
  return mMetaId;
}

auto LayerDelegate::get_ctx() -> ContextInfo&
{
  return mContext;
}

auto LayerDelegate::get_ctx() const -> const ContextInfo&
{
  return mContext;
}

auto LayerDelegate::clone() const -> LayerDelegate
{
  LayerDelegate result;

  result.mContext = mContext.clone();
  result.mParentId = mParentId;
  result.mOpacity = mOpacity;
  result.mVisible = mVisible;
  result.mMetaId = nothing;  // This has to be set separately

  return result;
}

}  // namespace tactile
