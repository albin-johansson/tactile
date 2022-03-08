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

#include "core/systems/component_system.hpp"

namespace tactile {

RenameComponentCmd::RenameComponentCmd(registry_ref registry,
                                       const component_id id,
                                       std::string name)
    : ACommand{"Rename Component"}
    , mRegistry{registry}
    , mComponentId{id}
    , mUpdatedName{std::move(name)}
{}

void RenameComponentCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::rename_component_def(registry, mComponentId, mPreviousName.value());
}

void RenameComponentCmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousName = sys::get_component_def_name(registry, mComponentId);
  sys::rename_component_def(registry, mComponentId, mUpdatedName);
}

}  // namespace tactile