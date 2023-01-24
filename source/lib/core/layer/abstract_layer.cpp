/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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