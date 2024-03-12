// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "undef_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

UndefComponent::UndefComponent(Document* document, const UUID& component_id)
    : mDocument {document},
      mComponentId {component_id}
{
  if (!mDocument) {
    throw Exception {"Invalid null document!"};
  }
}

void UndefComponent::undo()
{
  auto component_index = mDocument->get_component_index_ptr();

  component_index->restore_comp(std::move(mPreviousDef.value()));
  mPreviousDef.reset();

  // Restores previously removed components
  auto& context_manager = mDocument->get_contexts();
  for (auto [context_id, component]: mRemovedComponents) {
    auto& context = context_manager.get_context(context_id);
    context.get_ctx().attach_component(std::move(component));
  }

  mRemovedComponents.clear();
}

void UndefComponent::redo()
{
  auto component_index = mDocument->get_component_index_ptr();

  mPreviousDef = component_index->get_comp(mComponentId);
  component_index->remove_comp(mComponentId);

  auto& context_manager = mDocument->get_contexts();
  mRemovedComponents = context_manager.on_undef_comp(mComponentId);
}

auto UndefComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.undef_comp;
}

}  // namespace tactile::cmd
