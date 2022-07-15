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

#include "reset_attached_component_cmd.hpp"

#include <utility>  // move

#include "core/comp/component_bundle.hpp"
#include "core/comp/component_index.hpp"
#include "core/ctx/context.hpp"
#include "misc/panic.hpp"

namespace tactile {

ResetAttachedComponentCmd::ResetAttachedComponentCmd(Shared<ComponentIndex> index,
                                                     Shared<IContext>       context,
                                                     const UUID&            componentId)
    : mIndex {std::move(index)}
    , mContext {std::move(context)}
    , mComponentId {componentId}
{
  if (!mIndex) {
    throw TactileError {"Invalid null component index!"};
  }
  else if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void ResetAttachedComponentCmd::undo()
{
  auto& comps = mContext->get_comps();

  comps.erase(mComponentId);
  comps.add(mComponent.value());

  mComponent.reset();
}

void ResetAttachedComponentCmd::redo()
{
  auto&       comps = mContext->get_comps();
  const auto& definition = mIndex->at(mComponentId);

  mComponent = comps.at(mComponentId);

  comps.erase(mComponentId);
  comps.add(definition.instantiate());
}

auto ResetAttachedComponentCmd::get_name() const -> const char*
{
  return "Reset Component Values";
}

}  // namespace tactile