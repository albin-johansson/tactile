// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_component_attr.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"
#include "tactile/core/debug/exception.hpp"

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
    throw Exception {"Invalid null document!"};
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
