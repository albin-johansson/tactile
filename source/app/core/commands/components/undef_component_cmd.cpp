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

#include "undef_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_bundle.hpp"
#include "core/components/component_index.hpp"
#include "core/contexts/context_manager.hpp"
#include "core/documents/document.hpp"
#include "misc/panic.hpp"

namespace tactile {

UndefComponentCmd::UndefComponentCmd(ADocument* document, const UUID& componentId)
    : mDocument {document}
    , mComponentId {componentId}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void UndefComponentCmd::undo()
{
  auto index = mDocument->get_component_index();

  index->restore_comp(std::move(mPreviousDef.value()));
  mPreviousDef.reset();

  /* Restores previously removed components */
  for (auto [contextId, component] : mRemovedComponents) {
    auto context = mDocument->get_context(contextId);
    context->get_comps().add(std::move(component));
  }

  mRemovedComponents.clear();
}

void UndefComponentCmd::redo()
{
  auto index = mDocument->get_component_index();

  mPreviousDef = index->at(mComponentId);
  index->remove_comp(mComponentId);

  auto& contexts = mDocument->get_contexts();
  mRemovedComponents = contexts.on_undef_comp(mComponentId);
}

auto UndefComponentCmd::get_name() const -> const char*
{
  return "Remove Component Definition";
}

}  // namespace tactile