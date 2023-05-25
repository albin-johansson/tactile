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

#include "rename_object.hpp"

#include <utility>  // move

#include "components/context.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RenameObject::RenameObject(const Entity object_entity, String name)
    : mObjectEntity {object_entity},
      mNewName {std::move(name)}
{
}

void RenameObject::undo()
{
  auto& model = get_global_model();
  auto& object_context = model.get<Context>(mObjectEntity);

  object_context.name = mOldName.value();
  mOldName.reset();
}

void RenameObject::redo()
{
  auto& model = get_global_model();
  auto& object_context = model.get<Context>(mObjectEntity);

  mOldName = object_context.name;
  object_context.name = mNewName;
}

auto RenameObject::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameObject*>(cmd)) {
    if (mObjectEntity == other->mObjectEntity) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameObject::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.rename_object;
}

}  // namespace tactile::cmd
