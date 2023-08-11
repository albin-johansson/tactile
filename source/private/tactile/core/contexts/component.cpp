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

#include "tactile/core/contexts/component.hpp"

#include <utility>  // move

namespace tactile {

Component::Component(const ComponentID id, std::string name)
    : mID {id},
      mName {std::move(name)}
{
}

void Component::set_name(std::string name)
{
  mName = std::move(name);
}

auto Component::add_attribute(std::string name) -> Property&
{
  return mAttributes.add(std::move(name));
}

void Component::remove_attribute(std::string_view name)
{
  mAttributes.remove(name);
}

void Component::rename_attribute(std::string_view current_name, std::string new_name)
{
  mAttributes.rename(current_name, std::move(new_name));
}

void Component::update_attribute(std::string_view name, Property value)
{
  mAttributes.update(name, std::move(value));
}

auto Component::get_id() const -> ComponentID
{
  return mID;
}

auto Component::get_name() const -> const std::string&
{
  return mName;
}

auto Component::instantiate() const -> ComponentInstance
{
  return ComponentInstance {mID, mAttributes};
}

}  // namespace tactile