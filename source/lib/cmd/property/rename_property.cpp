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

#include "rename_property.hpp"

#include <utility>  // move

#include "model/context.hpp"
#include "model/systems/context/properties.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RenameProperty::RenameProperty(const Entity context_entity,
                               String old_name,
                               String new_name)
    : mContextEntity {context_entity},
      mOldName {std::move(old_name)},
      mNewName {std::move(new_name)}
{
}

void RenameProperty::undo()
{
  auto& context = get_global_model().get<Context>(mContextEntity);
  sys::rename_property(context, mNewName, mOldName);
}

void RenameProperty::redo()
{
  auto& context = get_global_model().get<Context>(mContextEntity);
  sys::rename_property(context, mOldName, mNewName);
}

auto RenameProperty::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.rename_property;
}

}  // namespace tactile::cmd
