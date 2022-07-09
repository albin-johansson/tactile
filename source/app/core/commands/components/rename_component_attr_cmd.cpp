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

#include "rename_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameComponentAttrCmd::RenameComponentAttrCmd(Shared<ComponentIndex> index,
                                               const UUID&            componentId,
                                               std::string            previousName,
                                               std::string            updatedName)
    : mIndex {std::move(index)}
    , mComponentId {componentId}
    , mPreviousName {std::move(previousName)}
    , mUpdatedName {std::move(updatedName)}
{
  if (!mIndex) {
    throw TactileError {"Invalid null component index!"};
  }
}

void RenameComponentAttrCmd::undo()
{
  auto& definition = mIndex->at(mComponentId);
  definition.rename_attr(mUpdatedName, mPreviousName);
}

void RenameComponentAttrCmd::redo()
{
  auto& definition = mIndex->at(mComponentId);
  definition.rename_attr(mPreviousName, mUpdatedName);
}

auto RenameComponentAttrCmd::get_name() const -> const char*
{
  return "Rename Component Attribute";
}

}  // namespace tactile