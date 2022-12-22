/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "layer_delegate.hpp"

#include <algorithm> // clamp

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