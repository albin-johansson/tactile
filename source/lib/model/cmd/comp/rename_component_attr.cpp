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

#include "rename_component_attr.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/component/component_index.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"

namespace tactile::cmd {

RenameComponentAttr::RenameComponentAttr(Document* document,
                                         const UUID& component_id,
                                         String previous_name,
                                         String updated_name)
    : mDocument {document},
      mComponentId {component_id},
      mPreviousName {std::move(previous_name)},
      mUpdatedName {std::move(updated_name)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void RenameComponentAttr::undo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  component_def.rename_attr(mUpdatedName, mPreviousName);

  auto& context_manager = mDocument->get_contexts();
  context_manager.on_renamed_component_attr(component_def.get_uuid(),
                                            mUpdatedName,
                                            mPreviousName);
}

void RenameComponentAttr::redo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  component_def.rename_attr(mPreviousName, mUpdatedName);

  auto& context_manager = mDocument->get_contexts();
  context_manager.on_renamed_component_attr(component_def.get_uuid(),
                                            mPreviousName,
                                            mUpdatedName);
}

auto RenameComponentAttr::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_comp_attr;
}

}  // namespace tactile::cmd