// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/add_property_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/document/document.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

AddPropertyCommand::AddPropertyCommand(IDocument* document,
                                       const UUID& context_uuid,
                                       String name,
                                       const AttributeType type)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mPropertyName {std::move(name)},
    mPropertyType {type}
{}

void AddPropertyCommand::undo()
{
  TACTILE_LOG_TRACE("[AddPropertyCommand::undo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& metadata = context->meta();
    metadata.remove_property(mPropertyName);
  }
  else {
    TACTILE_LOG_ERROR("[AddPropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void AddPropertyCommand::redo()
{
  TACTILE_LOG_TRACE("[AddPropertyCommand::redo] context: {}",
                    fmt::streamed(mContextUuid));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& metadata = context->meta();
    metadata.add_property(mPropertyName).reset(mPropertyType);
  }
  else {
    TACTILE_LOG_ERROR("[AddPropertyCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

}  // namespace tactile::core
