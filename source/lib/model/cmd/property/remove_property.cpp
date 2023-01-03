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

#include "remove_property.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

RemoveProperty::RemoveProperty(Shared<Context> context, String name)
    : mContext {std::move(context)},
      mName {std::move(name)}
{
  if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void RemoveProperty::undo()
{
  mContext->get_ctx().add_property(mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemoveProperty::redo()
{
  auto& ctx = mContext->get_ctx();
  mPreviousValue = ctx.get_property(mName);
  ctx.remove_property(mName);
}

auto RemoveProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_property;
}

}  // namespace tactile::cmd
