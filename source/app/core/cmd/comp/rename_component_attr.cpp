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

#include "core/comp/component_index.hpp"
#include "core/ctx/context_manager.hpp"
#include "core/document/document.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

RenameComponentAttr::RenameComponentAttr(ADocument* document,
                                         const UUID& component_id,
                                         std::string previous_name,
                                         std::string updated_name)
    : mDocument {document}
    , mComponentId {component_id}
    , mPreviousName {std::move(previous_name)}
    , mUpdatedName {std::move(updated_name)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }
}

void RenameComponentAttr::undo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  definition.rename_attr(mUpdatedName, mPreviousName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_renamed_component_attr(definition.get_uuid(), mUpdatedName, mPreviousName);
}

void RenameComponentAttr::redo()
{
  auto index = mDocument->get_component_index();

  auto& definition = index->at(mComponentId);
  definition.rename_attr(mPreviousName, mUpdatedName);

  auto& contexts = mDocument->get_contexts();
  contexts.on_renamed_component_attr(definition.get_uuid(), mPreviousName, mUpdatedName);
}

auto RenameComponentAttr::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_comp_attr;
}

}  // namespace tactile::cmd