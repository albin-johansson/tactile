// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "update_attached_component.hpp"

#include <utility>  // move

#include "core/component/component.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

UpdateAttachedComponent::UpdateAttachedComponent(Shared<Context> context,
                                                 const UUID& component_id,
                                                 String attribute,
                                                 Attribute value)
    : mContext {std::move(context)},
      mComponentId {component_id},
      mAttributeName {std::move(attribute)},
      mUpdatedValue {std::move(value)}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void UpdateAttachedComponent::undo()
{
  auto& component = mContext->get_ctx().get_component(mComponentId);
  component.update_attr(mAttributeName, mPreviousValue.value());

  mPreviousValue.reset();
}

void UpdateAttachedComponent::redo()
{
  auto& component = mContext->get_ctx().get_component(mComponentId);

  mPreviousValue = component.get_attr(mAttributeName);
  component.update_attr(mAttributeName, mUpdatedValue);
}

auto UpdateAttachedComponent::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateAttachedComponent*>(cmd)) {
    const bool can_merge = mContext->get_uuid() == other->mContext->get_uuid() &&
                           mComponentId == other->mComponentId &&
                           mAttributeName == other->mAttributeName;
    if (can_merge) {
      mUpdatedValue = other->mUpdatedValue;
      return true;
    }
  }

  return false;
}

auto UpdateAttachedComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.update_comp_attr;
}

}  // namespace tactile::cmd
