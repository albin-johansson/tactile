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

#include "common/debug/assert.hpp"
#include "components/context.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::cmd {

AddProperty::AddProperty(Model* model,
                         const Entity context_entity,
                         String name,
                         const AttributeType type)
    : mModel {model},
      mContextEntity {context_entity},
      mName {std::move(name)},
      mType {type}
{
  TACTILE_ASSERT(sys::is_context_entity(*mModel, mContextEntity));
}

void AddProperty::undo()
{
  auto& model = *mModel;

  auto& context = model.get<Context>(mContextEntity);
  erase_from(context.props, mName);
}

void AddProperty::redo()
{
  auto& model = *mModel;

  auto& context = model.get<Context>(mContextEntity);
  context.props[mName].reset_to_default(mType);
}

auto AddProperty::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.add_property;
}

}  // namespace tactile::cmd
