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

#include "core/comp/component_index.hpp"
#include "core/ctx/context_manager.hpp"
#include "core/document/document.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameComponentAttrCmd::RenameComponentAttrCmd(ADocument*  document,
                                               const UUID& componentId,
                                               std::string previousName,
                                               std::string updatedName)
    : mDocument {document}
    , mComponentId {componentId}
    , mPreviousName {std::move(previousName)}
    , mUpdatedName {std::move(updatedName)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void RenameComponentAttrCmd::undo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  definition.rename_attr(mUpdatedName, mPreviousName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_renamed_component_attr(definition.get_uuid(), mUpdatedName, mPreviousName);
}

void RenameComponentAttrCmd::redo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  definition.rename_attr(mPreviousName, mUpdatedName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_renamed_component_attr(definition.get_uuid(), mPreviousName, mUpdatedName);
}

auto RenameComponentAttrCmd::get_name() const -> const char*
{
  return "Rename Component Attribute";
}

}  // namespace tactile