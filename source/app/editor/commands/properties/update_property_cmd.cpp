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

#include "update_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

UpdatePropertyCmd::UpdatePropertyCmd(RegistryRef registry,
                                     std::string name,
                                     Attribute value)
    : ACommand{"update Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
    , mNewValue{std::move(value)}
{}

void UpdatePropertyCmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::update_property(mRegistry, context, mName, mOldValue.value());
  mOldValue.reset();
}

void UpdatePropertyCmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  mOldValue = sys::get_property(mRegistry, context, mName).value;
  sys::update_property(mRegistry, context, mName, mNewValue);
}

auto UpdatePropertyCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const UpdatePropertyCmd&>(cmd);
    if (mContextId == other.mContextId && mName == other.mName) {
      mNewValue = other.mNewValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
