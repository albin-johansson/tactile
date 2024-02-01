// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_property_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/document/document.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

RenamePropertyCommand::RenamePropertyCommand(IDocument* document,
                                             const UUID& context_uuid,
                                             String old_property_name,
                                             String new_property_name)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mOldPropertyName {std::move(old_property_name)},
    mNewPropertyName {std::move(new_property_name)}
{}

void RenamePropertyCommand::undo()
{
  TACTILE_LOG_TRACE("[RenamePropertyCommand::undo] context: {}, name: '{}' -> '{}'",
                    fmt::streamed(mContextUuid),
                    mNewPropertyName,
                    mOldPropertyName);

  if (auto* context = mDocument->find_context(mContextUuid)) {
    context->meta().rename_property(mNewPropertyName, mOldPropertyName);
  }
  else {
    TACTILE_LOG_ERROR("[RenamePropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void RenamePropertyCommand::redo()
{
  TACTILE_LOG_TRACE("[RenamePropertyCommand::redo] context: {}, name: '{}' -> '{}'",
                    fmt::streamed(mContextUuid),
                    mOldPropertyName,
                    mNewPropertyName);

  if (auto* context = mDocument->find_context(mContextUuid)) {
    context->meta().rename_property(mOldPropertyName, mNewPropertyName);
  }
  else {
    TACTILE_LOG_ERROR("[RenamePropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

}  // namespace tactile::core
