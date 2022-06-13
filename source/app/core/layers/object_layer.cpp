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

#include "object_layer.hpp"

#include "core/layers/layer_visitor.hpp"

namespace tactile::core {

auto ObjectLayer::make() -> Shared<ObjectLayer>
{
  return std::make_shared<ObjectLayer>();
}

void ObjectLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void ObjectLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto ObjectLayer::object_count() const -> usize
{
  return mObjects.size();
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
  auto copy = make();
  copy->mDelegate = mDelegate.clone();
  return copy;
}

auto ObjectLayer::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto ObjectLayer::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto ObjectLayer::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto ObjectLayer::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

auto ObjectLayer::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

}  // namespace tactile::core
