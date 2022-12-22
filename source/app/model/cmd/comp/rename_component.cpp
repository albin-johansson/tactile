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

#include "rename_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

RenameComponent::RenameComponent(Shared<ComponentIndex> index,
                                 const UUID& component_id,
                                 String name)
    : mIndex {std::move(index)},
      mComponentId {component_id},
      mUpdatedName {std::move(name)}
{
  if (!mIndex) {
    throw TactileError {"Invalid null component index!"};
  }
}

void RenameComponent::undo()
{
  mIndex->rename(mComponentId, mPreviousName.value());
  mPreviousName.reset();
}

void RenameComponent::redo()
{
  const auto& definition = mIndex->at(mComponentId);
  mPreviousName = definition.name();
  mIndex->rename(mComponentId, mUpdatedName);
}

auto RenameComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_comp;
}

}  // namespace tactile::cmd