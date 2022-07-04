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

#include "remove_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_bundle.hpp"
#include "core/contexts/context.hpp"
#include "misc/panic.hpp"

namespace tactile {

DetachComponentCmd::DetachComponentCmd(Shared<core::IContext> context,
                                       const UUID&            componentId)
    : mContext{std::move(context)}
    , mComponentId{componentId}
{
  if (!mContext) {
    throw TactileError{"Invalid null context!"};
  }
}

void DetachComponentCmd::undo()
{
  auto& comps = mContext->get_comps();
  comps.add(mComponent.value());
  mComponent.reset();
}

void DetachComponentCmd::redo()
{
  auto& comps = mContext->get_comps();
  mComponent = comps.erase(mComponentId);
}

auto DetachComponentCmd::get_name() const -> const char*
{
  return "Remove Component";
}

}  // namespace tactile