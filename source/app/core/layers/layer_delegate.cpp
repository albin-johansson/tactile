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

#include <algorithm>  // clamp

namespace tactile::core {

LayerDelegate::LayerDelegate() : mId{make_uuid()} {}

void LayerDelegate::set_opacity(const float opacity)
{
  mOpacity = std::clamp(opacity, 0.0f, 1.0f);
}

void LayerDelegate::set_visible(const bool visible)
{
  mVisible = visible;
}

auto LayerDelegate::get_opacity() const -> float
{
  return mOpacity;
}

auto LayerDelegate::is_visible() const -> bool
{
  return mVisible;
}

auto LayerDelegate::get_uuid() const -> const UUID&
{
  return mId;
}

auto LayerDelegate::get_props() -> PropertyBundle&
{
  return mProps;
}

auto LayerDelegate::get_props() const -> const PropertyBundle&
{
  return mProps;
}

auto LayerDelegate::get_comps() -> ComponentBundle&
{
  return mComps;
}

auto LayerDelegate::get_comps() const -> const ComponentBundle&
{
  return mComps;
}

auto LayerDelegate::clone() const -> LayerDelegate
{
  LayerDelegate result;

  result.mId = make_uuid();
  result.mProps = mProps;
  result.mComps = mComps;
  result.mOpacity = mOpacity;
  result.mVisible = mVisible;

  return result;
}

}  // namespace tactile::core