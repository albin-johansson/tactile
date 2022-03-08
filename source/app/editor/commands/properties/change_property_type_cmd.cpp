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

#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

change_property_type_cmd::change_property_type_cmd(registry_ref registry,
                                                   std::string name,
                                                   const attribute_type type)
    : ACommand{"Change Property Type"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
    , mPropertyType{type}
{}

void change_property_type_cmd::undo()
{
  const auto& value = mPreviousValue.value();
  const auto type = value.type();

  auto& context = sys::get_context(mRegistry, mContextId);
  sys::change_property_type(mRegistry, context, mName, type);
  sys::update_property(mRegistry, context, mName, value);

  mPreviousValue.reset();
}

void change_property_type_cmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  mPreviousValue = sys::get_property(mRegistry, context, mName).value;
  sys::change_property_type(mRegistry, context, mName, mPropertyType);
}

}  // namespace tactile
