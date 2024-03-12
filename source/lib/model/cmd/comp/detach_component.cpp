// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "detach_component.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

DetachComponent::DetachComponent(Shared<Context> context, const UUID& component_id)
    : mContext {std::move(context)},
      mComponentId {component_id}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void DetachComponent::undo()
{
  mContext->get_ctx().attach_component(mComponent.value());
  mComponent.reset();
}

void DetachComponent::redo()
{
  mComponent = mContext->get_ctx().detach_component(mComponentId);
}

auto DetachComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.detach_comp;
}

}  // namespace tactile::cmd
