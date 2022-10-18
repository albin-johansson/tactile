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

#include "duplicate_component_attr.hpp"

#include <utility>  // move

#include "core/comp/component_index.hpp"
#include "core/ctx/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"
#include "model/document/document.hpp"

namespace tactile::cmd {

DuplicateComponentAttr::DuplicateComponentAttr(Document* document,
                                               const UUID& component_id,
                                               String attribute)
    : mDocument {document},
      mComponentId {component_id},
      mAttributeName {std::move(attribute)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void DuplicateComponentAttr::undo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  definition.remove(mDuplicatedName.value());

  auto& contexts = mDocument->get_contexts();
  contexts.on_removed_component_attr(definition.uuid(), mDuplicatedName.value());

  mDuplicatedName.reset();
}

void DuplicateComponentAttr::redo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  mDuplicatedName = definition.duplicate(mAttributeName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_new_component_attr(definition.uuid(),
                                 *mDuplicatedName,
                                 definition.at(*mDuplicatedName));
}

auto DuplicateComponentAttr::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.duplicate_comp_attr;
}

}  // namespace tactile::cmd