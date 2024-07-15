// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/update_property_command.hpp"

#include <utility>  // move, exchange

#include "tactile/core/debug/validation.hpp"
#include "tactile/base/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

UpdatePropertyCommand::UpdatePropertyCommand(IDocument* document,
                                             const EntityID context_entity,
                                             String property_name,
                                             Attribute property_value)
  : mDocument {require_not_null(document, "null document")},
    mContextEntity {context_entity},
    mPropertyName {std::move(property_name)},
    mNewPropertyValue {std::move(property_value)}
{}

void UpdatePropertyCommand::undo()
{
  TACTILE_SET_LOG_SCOPE("UpdatePropertyCommand");
  TACTILE_LOG_DEBUG("Reverting update of property '{}'", mPropertyName);

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  auto& value = lookup_in(meta.properties, mPropertyName);
  value = mOldPropertyValue.value();
}

void UpdatePropertyCommand::redo()
{
  TACTILE_SET_LOG_SCOPE("UpdatePropertyCommand");
  TACTILE_LOG_DEBUG("Updating the value of property '{}'", mPropertyName);

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  auto& value = lookup_in(meta.properties, mPropertyName);
  mOldPropertyValue = std::exchange(value, mNewPropertyValue);
}

auto UpdatePropertyCommand::merge_with(const ICommand* cmd) -> bool
{
  const auto* that = dynamic_cast<const UpdatePropertyCommand*>(cmd);
  if (!that) {
    return false;
  }

  const auto same_target = this->mDocument == that->mDocument &&
                           this->mContextEntity == that->mContextEntity &&
                           this->mPropertyName == that->mPropertyName;
  if (!same_target) {
    return false;
  }

  TACTILE_SET_LOG_SCOPE("UpdatePropertyCommand");
  TACTILE_LOG_TRACE("Merging update command for property '{}'", mPropertyName);

  this->mNewPropertyValue = that->mNewPropertyValue;
  return true;
}

}  // namespace tactile
