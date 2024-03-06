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

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameProperty::RenameProperty(Shared<Context> context, String old_name, String new_name)
    : mContext {std::move(context)},
      mOldName {std::move(old_name)},
      mNewName {std::move(new_name)}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void RenameProperty::undo()
{
  mContext->get_ctx().rename_property(mNewName, mOldName);
}

void RenameProperty::redo()
{
  mContext->get_ctx().rename_property(mOldName, mNewName);
}

auto RenameProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_property;
}

}  // namespace tactile::cmd
