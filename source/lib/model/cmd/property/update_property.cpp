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

#include "update_property.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

UpdateProperty::UpdateProperty(Shared<Context> context, String name, Attribute value)
    : mContext {std::move(context)},
      mName {std::move(name)},
      mNewValue {std::move(value)}
{
  if (!mContext) {
    throw Error {"Invalid null context!"};
  }
}

void UpdateProperty::undo()
{
  mContext->get_ctx().update_property(mName, mOldValue.value());
  mOldValue.reset();
}

void UpdateProperty::redo()
{
  auto& ctx = mContext->get_ctx();
  mOldValue = ctx.get_property(mName);
  ctx.update_property(mName, mNewValue);
}

auto UpdateProperty::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateProperty*>(cmd)) {
    if (mContext->get_uuid() == other->mContext->get_uuid() && mName == other->mName) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.update_property;
}

}  // namespace tactile::cmd
