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

#include "rename_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameComponentCmd::RenameComponentCmd(Shared<ComponentIndex> index,
                                       const UUID&            componentId,
                                       std::string            name)
    : mIndex{std::move(index)}
    , mComponentId{componentId}
    , mUpdatedName{std::move(name)}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
}

void RenameComponentCmd::undo()
{
  mIndex->rename_comp(mComponentId, mPreviousName.value());
  mPreviousName.reset();
}

void RenameComponentCmd::redo()
{
  const auto& definition = mIndex->at(mComponentId);
  mPreviousName = definition.get_name();
  mIndex->rename_comp(mComponentId, mUpdatedName);
}

auto RenameComponentCmd::get_name() const -> const char*
{
  return "Rename Component";
}

}  // namespace tactile