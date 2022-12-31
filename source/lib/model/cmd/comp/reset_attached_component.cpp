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

#include "reset_attached_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

ResetAttachedComponent::ResetAttachedComponent(Shared<ComponentIndex> index,
                                               Shared<Context> context,
                                               const UUID& component_id)
    : mIndex {std::move(index)},
      mContext {std::move(context)},
      mComponentId {component_id}
{
  if (!mIndex) {
    throw TactileError {"Invalid null component index!"};
  }
  else if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void ResetAttachedComponent::undo()
{
  auto& comps = mContext->get_ctx().comps();

  comps.erase(mComponentId);
  comps.add(mComponent.value());

  mComponent.reset();
}

void ResetAttachedComponent::redo()
{
  auto& comps = mContext->get_ctx().comps();
  const auto& definition = mIndex->at(mComponentId);

  mComponent = comps.at(mComponentId);

  comps.erase(mComponentId);
  comps.add(definition.instantiate());
}

auto ResetAttachedComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.reset_comp;
}

}  // namespace tactile::cmd