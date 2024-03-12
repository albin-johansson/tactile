// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_component_attr_type.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"
#include "tactile/core/debug/exception.hpp"

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
    throw Exception {"Invalid null document!"};
  }
}

void SetComponentAttrType::undo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  component_def.remove_attr(mAttributeName);
  component_def.add_attr(mAttributeName, mSnapshot.value());

  auto& context_manager = mDocument->get_contexts();
  for (const auto& [context_id, attribute_value]: mPrevAttributes) {
    auto& context = context_manager.get_context(context_id);
    auto& component = context.get_ctx().get_component(component_def.get_uuid());
    component.remove_attr(mAttributeName);
    component.add_attr(mAttributeName, attribute_value);
  }

  mSnapshot.reset();
}

void SetComponentAttrType::redo()
{
  auto component_index = mDocument->get_component_index_ptr();

  auto& component_def = component_index->get_comp(mComponentId);
  mSnapshot = component_def.get_attr(mAttributeName);

  component_def.remove_attr(mAttributeName);
  component_def.add_attr(mAttributeName, mNewType);

  auto& context_manager = mDocument->get_contexts();
  mPrevAttributes =
      context_manager.on_changed_component_attr_type(component_def.get_uuid(),
                                                     mAttributeName,
                                                     mNewType);
}

auto SetComponentAttrType::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.change_comp_attr_type;
}

}  // namespace tactile::cmd
