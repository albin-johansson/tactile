// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/update_property_type_command.hpp"

#include <utility>  // move, to_underlying

#include <fmt/ostream.h>

#include "tactile/core/document/document.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

UpdatePropertyTypeCommand::UpdatePropertyTypeCommand(IDocument* document,
                                                     const UUID& context_uuid,
                                                     String property_name,
                                                     const AttributeType property_type)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mPropertyName {std::move(property_name)},
    mNewPropertyType {property_type}
{}

void UpdatePropertyTypeCommand::undo()
{
  TACTILE_LOG_TRACE("[UpdatePropertyTypeCommand::undo] context: {}, new type: {}",
                    fmt::streamed(mContextUuid),
                    std::to_underlying(mNewPropertyType));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& property_value = context->meta().get_property(mPropertyName);

    property_value = std::move(mOldPropertyValue.value());
    mOldPropertyValue.reset();
  }
  else {
    TACTILE_LOG_ERROR("[UpdatePropertyTypeCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void UpdatePropertyTypeCommand::redo()
{
  TACTILE_LOG_TRACE("[UpdatePropertyTypeCommand::redo] context: {}, new type: {}",
                    fmt::streamed(mContextUuid),
                    std::to_underlying(mNewPropertyType));

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& property_value = context->meta().get_property(mPropertyName);

    mOldPropertyValue = property_value;
    property_value.reset(mNewPropertyType);
  }
  else {
    TACTILE_LOG_ERROR("[UpdatePropertyTypeCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

}  // namespace tactile
