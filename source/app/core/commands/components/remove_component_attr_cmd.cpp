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

#include "remove_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/components/component_index.hpp"
#include "core/contexts/context_manager.hpp"
#include "core/documents/document.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveComponentAttrCmd::RemoveComponentAttrCmd(ADocument*  document,
                                               const UUID& componentId,
                                               std::string attribute)
    : mDocument {document}
    , mComponentId {componentId}
    , mAttributeName {std::move(attribute)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
  else if (!mDocument->get_component_index()) {
    throw TactileError {"Invalid null component index!"};
  }
}

void RemoveComponentAttrCmd::undo()
{
  auto  index = mDocument->get_component_index();
  auto& definition = index->at(mComponentId);

  auto value = mPreviousValue.value();
  definition.add_attr(mAttributeName, value);

  auto& contexts = mDocument->get_contexts();
  contexts.on_new_component_attr(definition.get_uuid(), mAttributeName, value);

  mPreviousValue.reset();
}

void RemoveComponentAttrCmd::redo()
{
  auto  index = mDocument->get_component_index();
  auto& definition = index->at(mComponentId);

  mPreviousValue = definition.get_attr(mAttributeName);
  definition.remove_attr(mAttributeName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_removed_component_attr(definition.get_uuid(), mAttributeName);
}

auto RemoveComponentAttrCmd::get_name() const -> const char*
{
  return "Remove Component Attribute";
}

}  // namespace tactile