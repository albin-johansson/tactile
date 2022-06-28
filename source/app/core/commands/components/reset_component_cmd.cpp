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

#include "reset_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_bundle.hpp"
#include "core/components/component_definition.hpp"
#include "core/components/component_index.hpp"
#include "core/context.hpp"
#include "misc/panic.hpp"

namespace tactile {

ResetComponentCmd::ResetComponentCmd(Shared<core::ComponentIndex> index,
                                     Shared<core::IContext>       context,
                                     const UUID&                  contextId,
                                     const UUID&                  componentId)
    : ACommand{"Reset Component Values"}
    , mIndex{std::move(index)}
    , mContext{std::move(context)}
    , mContextId{contextId}
    , mComponentId{componentId}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
  else if (!mContext) {
    throw TactileError{"Invalid null context!"};
  }
}

void ResetComponentCmd::undo()
{
  auto& comps = mContext->get_comps();

  comps.erase(mComponentId);
  comps.add(mComponent.value());

  mComponent.reset();
}

void ResetComponentCmd::redo()
{
  auto&       comps = mContext->get_comps();
  const auto& definition = mIndex->at(mComponentId);

  mComponent = comps.at(mComponentId);

  comps.erase(mComponentId);
  comps.add(definition.instantiate());
}

}  // namespace tactile