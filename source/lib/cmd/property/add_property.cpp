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

#include "add_property.hpp"

#include <utility>  // move

#include "components/context.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

AddProperty::AddProperty(const Entity context_entity,
                         String name,
                         const AttributeType type)
    : mContextEntity {context_entity},
      mName {std::move(name)},
      mType {type}
{
}

void AddProperty::undo()
{
  auto& model = get_global_model();
  auto& context = model.get<Context>(mContextEntity);
  erase_from(context.props, mName);
}

void AddProperty::redo()
{
  auto& model = get_global_model();
  auto& context = model.get<Context>(mContextEntity);
  context.props[mName].reset_to_default(mType);
}

auto AddProperty::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.add_property;
}

}  // namespace tactile::cmd
