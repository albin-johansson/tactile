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

#include "define_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

DefineComponent::DefineComponent(Shared<ComponentIndex> index, String name)
    : mIndex {std::move(index)},
      mName {std::move(name)}
{
  if (!mIndex) {
    throw Error {"Invalid null component index!"};
  }
}

void DefineComponent::undo()
{
  const auto& component_def = mDef.value();
  mIndex->remove_comp(component_def.get_uuid());
}

void DefineComponent::redo()
{
  if (mDef.has_value()) {
    mIndex->restore_comp(*mDef);
  }
  else {
    const auto component_id = mIndex->define_comp(mName);
    mDef = mIndex->get_comp(component_id);
  }
}

auto DefineComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.define_comp;
}

}  // namespace tactile::cmd