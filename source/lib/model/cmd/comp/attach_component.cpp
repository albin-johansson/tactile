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

#include "attach_component.hpp"

#include "common/debug/panic.hpp"
#include "core/component/component_index.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

AttachComponent::AttachComponent(Shared<ComponentIndex> index,
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

void AttachComponent::undo()
{
  mContext->get_ctx().detach_component(mComponentId);
}

void AttachComponent::redo()
{
  const auto& component_def = mIndex->get_comp(mComponentId);
  mContext->get_ctx().attach_component(component_def.instantiate());
}

auto AttachComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.attach_comp;
}

}  // namespace tactile::cmd