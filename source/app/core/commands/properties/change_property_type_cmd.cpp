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

#include "core/contexts/context.hpp"
#include "core/property_bundle.hpp"
#include "misc/panic.hpp"

namespace tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(Shared<core::IContext> context,
                                             std::string            name,
                                             const AttributeType    type)
    : mContext{std::move(context)}
    , mName{std::move(name)}
    , mPropertyType{type}
{
  if (!mContext) {
    throw TactileError{"Invalid null context!"};
  }
}

void ChangePropertyTypeCmd::undo()
{
  auto& props = mContext->get_props();

  const auto& value = mPreviousValue.value();
  const auto  type = value.type();

  props.change_type(mName, type);
  props.update(mName, value);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::redo()
{
  auto& props = mContext->get_props();
  mPreviousValue = props.at(mName);
  props.change_type(mName, mPropertyType);
}

auto ChangePropertyTypeCmd::get_name() const -> const char*
{
  return "Change Property Type";
}

}  // namespace tactile
