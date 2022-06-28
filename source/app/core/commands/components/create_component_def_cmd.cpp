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

#include "create_component_def_cmd.hpp"

#include <utility>  // move

#include "core/components/component_definition.hpp"
#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

CreateComponentDefCmd::CreateComponentDefCmd(Shared<core::ComponentIndex> index,
                                             std::string                  name)
    : ACommand{"Create Component Definition"}
    , mIndex{std::move(index)}
    , mName{std::move(name)}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
}

void CreateComponentDefCmd::undo()
{
  mIndex->remove_comp(mComponentId.value());
}

void CreateComponentDefCmd::redo()
{
  if (mComponentId) {
    mIndex->define_comp(*mComponentId, mName);
  }
  else {
    mComponentId = mIndex->define_comp(mName);
  }
}

auto CreateComponentDefCmd::get_name() const -> const char*
{
  return "Create Component Definition";
}

}  // namespace tactile
