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

#include "update_property.hpp"

#include <utility>  // move

#include "core/ctx/context.hpp"
#include "core/ctx/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

UpdateProperty::UpdateProperty(Shared<Context> context, std::string name, Attribute value)
    : mContext {std::move(context)}
    , mName {std::move(name)}
    , mNewValue {std::move(value)}
{
  if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void UpdateProperty::undo()
{
  auto& props = mContext->ctx().props();
  props.update(mName, mOldValue.value());
  mOldValue.reset();
}

void UpdateProperty::redo()
{
  auto& props = mContext->ctx().props();
  mOldValue = props.at(mName);
  props.update(mName, mNewValue);
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

auto UpdateProperty::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.update_property;
}

}  // namespace tactile::cmd
