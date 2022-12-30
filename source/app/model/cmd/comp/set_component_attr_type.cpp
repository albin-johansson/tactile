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

#include "set_component_attr_type.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"

namespace tactile::cmd {

SetComponentAttrType::SetComponentAttrType(Document* document,
                                           const UUID& component_id,
                                           String attribute,
                                           const AttributeType type)
    : mDocument {document},
      mComponentId {component_id},
      mAttributeName {std::move(attribute)},
      mNewType {type}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void SetComponentAttrType::undo()
{
  auto index = mDocument->get_component_index_ptr();
  auto& definition = index->at(mComponentId);

  definition.remove(mAttributeName);
  definition.add(mAttributeName, mSnapshot.value());

  auto& contexts = mDocument->get_contexts();
  for (const auto& [contextId, attribute]: mPrevAttributes) {
    auto& context = contexts.at(contextId);
    auto& comp = context.get_ctx().comps().at(definition.get_uuid());

    comp.remove(mAttributeName);
    comp.add(mAttributeName, attribute);
  }

  mSnapshot.reset();
}

void SetComponentAttrType::redo()
{
  auto index = mDocument->get_component_index_ptr();
  auto& definition = index->at(mComponentId);

  mSnapshot = definition.at(mAttributeName);

  definition.remove(mAttributeName);
  definition.add(mAttributeName, mNewType);

  auto& contexts = mDocument->get_contexts();
  mPrevAttributes = contexts.on_changed_component_attr_type(definition.get_uuid(),
                                                            mAttributeName,
                                                            mNewType);
}

auto SetComponentAttrType::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.change_comp_attr_type;
}

}  // namespace tactile::cmd