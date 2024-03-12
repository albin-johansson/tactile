// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "reset_attached_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

ResetAttachedComponent::ResetAttachedComponent(Shared<ComponentIndex> index,
                                               Shared<Context> context,
                                               const UUID& component_id)
    : mComponentIndex {std::move(index)},
      mContext {std::move(context)},
      mComponentId {component_id}
{
  if (!mComponentIndex) {
    throw Exception {"Invalid null component index!"};
  }
  else if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void ResetAttachedComponent::undo()
{
  auto& ctx = mContext->get_ctx();

  ctx.detach_component(mComponentId);
  ctx.attach_component(mPreviousComponent.value());

  mPreviousComponent.reset();
}

void ResetAttachedComponent::redo()
{
  auto& ctx = mContext->get_ctx();
  mPreviousComponent = ctx.get_component(mComponentId);

  const auto& component_def = mComponentIndex->get_comp(mComponentId);
  ctx.detach_component(mComponentId);
  ctx.attach_component(component_def.instantiate());
}

auto ResetAttachedComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.reset_comp;
}

}  // namespace tactile::cmd
