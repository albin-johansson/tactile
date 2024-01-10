// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/remove_property_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/document/document.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

RemovePropertyCommand::RemovePropertyCommand(IDocument* document,
                                             const UUID& context_uuid,
                                             String property_name)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mPropertyName {std::move(property_name)}
{}

void RemovePropertyCommand::undo()
{
  TACTILE_LOG_TRACE("[RemovePropertyCommand::undo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& property_value = context->meta().add_property(mPropertyName);

    if (mOldPropertyValue.has_value()) {
      property_value = std::move(*mOldPropertyValue);
      mOldPropertyValue.reset();
    }
  }
  else {
    TACTILE_LOG_ERROR("[RemovePropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void RemovePropertyCommand::redo()
{
  TACTILE_LOG_TRACE("[RemovePropertyCommand::redo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    mOldPropertyValue = context->meta().remove_property(mPropertyName);
  }
  else {
    TACTILE_LOG_ERROR("[RemovePropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

}  // namespace tactile
