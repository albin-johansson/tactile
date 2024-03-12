// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "attach_component.hpp"

#include "core/component/component_index.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AttachComponent::AttachComponent(Shared<ComponentIndex> index,
                                 Shared<Context> context,
                                 const UUID& component_id)
    : mIndex {std::move(index)},
      mContext {std::move(context)},
      mComponentId {component_id}
{
  if (!mIndex) {
    throw Exception {"Invalid null component index!"};
  }
  else if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void AttachComponent::undo()
{
  mContext->get_ctx().detach_component(mComponentId);
}

void AttachComponent::redo()
{
  const auto& component_def = mIndex->get_comp(mComponentId);
  mContext->get_ctx().attach_component(component_def.instantiate());
}

auto AttachComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.attach_comp;
}

}  // namespace tactile::cmd
