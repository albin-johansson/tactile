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

#include "set_object_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"

namespace tactile {

SetObjectNameCmd::SetObjectNameCmd(registry_ref registry,
                                   const object_id id,
                                   std::string name)
    : AObjectCommand{"Set Object Name", registry, id}
    , mNewName{std::move(name)}
{}

void SetObjectNameCmd::undo()
{
  auto& context = target_object_context();
  context.name = mOldName.value();
}

void SetObjectNameCmd::redo()
{
  auto& context = target_object_context();
  mOldName = context.name;
  context.name = mNewName;
}

auto SetObjectNameCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetObjectNameCmd&>(cmd);
    if (target_object_id() == other.target_object_id()) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
