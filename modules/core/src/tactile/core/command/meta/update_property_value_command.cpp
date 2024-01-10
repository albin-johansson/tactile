// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/update_property_value_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/document/document.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

UpdatePropertyValueCommand::UpdatePropertyValueCommand(IDocument* document,
                                                       const UUID& context_uuid,
                                                       String property_name,
                                                       Attribute property_value)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mPropertyName {std::move(property_name)},
    mNewPropertyValue {std::move(property_value)}
{}

void UpdatePropertyValueCommand::undo()
{
  TACTILE_LOG_TRACE("[UpdatePropertyValueCommand::undo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& property_value = context->meta().get_property(mPropertyName);

    property_value = std::move(mOldPropertyValue.value());
    mOldPropertyValue.reset();
  }
  else {
    TACTILE_LOG_ERROR("[UpdatePropertyValueCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void UpdatePropertyValueCommand::redo()
{
  TACTILE_LOG_TRACE("[UpdatePropertyValueCommand::redo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& property_value = context->meta().get_property(mPropertyName);

    mOldPropertyValue = property_value;
    property_value = mNewPropertyValue;
  }
  else {
    TACTILE_LOG_ERROR("[UpdatePropertyValueCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

auto UpdatePropertyValueCommand::merge_with(const ICommand* other) -> bool
{
  if (const auto* that = dynamic_cast<const UpdatePropertyValueCommand*>(other)) {
    if (this->mDocument == that->mDocument &&        //
        this->mContextUuid == that->mContextUuid &&  //
        this->mPropertyName == that->mPropertyName) {
      mNewPropertyValue = that->mNewPropertyValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
