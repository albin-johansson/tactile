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

#include "change_property_type.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

ChangePropertyType::ChangePropertyType(Shared<Context> context,
                                       String name,
                                       const AttributeType type)
    : mContext {std::move(context)},
      mName {std::move(name)},
      mPropertyType {type}
{
  if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void ChangePropertyType::undo()
{
  mContext->get_ctx().update_property(mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void ChangePropertyType::redo()
{
  auto& ctx = mContext->get_ctx();

  mPreviousValue = ctx.get_property(mName);
  ctx.update_property(mName, Attribute {mPropertyType});
}

auto ChangePropertyType::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.change_property_type;
}

}  // namespace tactile::cmd
