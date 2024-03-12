// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "duplicate_component_attr.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

DuplicateComponentAttr::DuplicateComponentAttr(Document* document,
                                               const UUID& component_id,
                                               String attribute)
    : mDocument {document},
      mComponentId {component_id},
      mAttributeName {std::move(attribute)}
{
  if (!mDocument) {
    throw Exception {"Invalid null document!"};
  }
}

void DuplicateComponentAttr::undo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  component_def.remove_attr(mDuplicatedName.value());

  auto& context_manager = mDocument->get_contexts();
  context_manager.on_removed_component_attr(component_def.get_uuid(),
                                            mDuplicatedName.value());

  mDuplicatedName.reset();
}

void DuplicateComponentAttr::redo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  mDuplicatedName = component_def.duplicate_attr(mAttributeName);

  auto& context_manager = mDocument->get_contexts();
  context_manager.on_new_component_attr(component_def.get_uuid(),
                                        *mDuplicatedName,
                                        component_def.get_attr(*mDuplicatedName));
}

auto DuplicateComponentAttr::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.duplicate_comp_attr;
}

}  // namespace tactile::cmd
