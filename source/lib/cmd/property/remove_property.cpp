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

#include "remove_property.hpp"

#include <utility>  // move

#include "common/util/assoc.hpp"
#include "core/context.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"

namespace tactile::cmd {

RemoveProperty::RemoveProperty(const Entity context_entity, String name)
    : mContextEntity {context_entity},
      mName {std::move(name)}
{
}

void RemoveProperty::undo()
{
  auto& model = get_model();

  auto& context = model.get<Context>(mContextEntity);
  context.props[mName] = mPreviousValue.value();

  mPreviousValue.reset();
}

void RemoveProperty::redo()
{
  auto& model = get_model();
  auto& context = model.get<Context>(mContextEntity);

  mPreviousValue = lookup_in(context.props, mName);
  context.props.erase(mName);
}

auto RemoveProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_property;
}

}  // namespace tactile::cmd