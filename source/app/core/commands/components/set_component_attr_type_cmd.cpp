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

#include "set_component_attr_type_cmd.hpp"

#include <utility>  // move

#include "core/components/component_bundle.hpp"
#include "core/components/component_index.hpp"
#include "core/contexts/context_manager.hpp"
#include "core/documents/document.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetComponentAttrTypeCmd::SetComponentAttrTypeCmd(ADocument*          document,
                                                 const UUID&         componentId,
                                                 std::string         attribute,
                                                 const AttributeType type)
    : mDocument {document}
    , mComponentId {componentId}
    , mAttributeName {std::move(attribute)}
    , mNewType {type}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void SetComponentAttrTypeCmd::undo()
{
  auto  index = mDocument->get_component_index();
  auto& definition = index->at(mComponentId);

  definition.remove_attr(mAttributeName);
  definition.add_attr(mAttributeName, mSnapshot.value());

  auto& contexts = mDocument->get_contexts();
  for (const auto& [contextId, attribute] : mPrevAttributes) {
    auto  context = mDocument->get_context(contextId);
    auto& comp = context->get_comps().at(definition.get_uuid());

    comp.remove_attr(mAttributeName);
    comp.add_attr(mAttributeName, attribute);
  }

  mSnapshot.reset();
}

void SetComponentAttrTypeCmd::redo()
{
  auto  index = mDocument->get_component_index();
  auto& definition = index->at(mComponentId);

  mSnapshot = definition.get_attr(mAttributeName);

  definition.remove_attr(mAttributeName);
  definition.add_attr(mAttributeName, mNewType);

  auto& contexts = mDocument->get_contexts();
  mPrevAttributes = contexts.on_changed_component_attr_type(definition.get_uuid(),
                                                            mAttributeName,
                                                            mNewType);
}

auto SetComponentAttrTypeCmd::get_name() const -> const char*
{
  return "Change Component Attribute Type";
}

}  // namespace tactile