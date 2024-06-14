// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/create_property_command.hpp"

#include <utility>  // move

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

CreatePropertyCommand::CreatePropertyCommand(IDocument* document,
                                             const EntityID context_entity,
                                             String name,
                                             Attribute value)
  : mDocument {require_not_null(document, "null document")},
    mContextEntity {context_entity},
    mName {std::move(name)},
    mValue {std::move(value)}
{}

void CreatePropertyCommand::undo()
{
  TACTILE_SET_LOG_SCOPE("CreatePropertyCommand");
  TACTILE_LOG_DEBUG("Removing property '{}' from entity {}",
                    mName,
                    entity_to_string(mContextEntity));

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  erase_from(meta.properties, mName);
}

void CreatePropertyCommand::redo()
{
  TACTILE_SET_LOG_SCOPE("CreatePropertyCommand");
  TACTILE_LOG_DEBUG("Adding property '{}' to entity {}",
                    mName,
                    entity_to_string(mContextEntity));

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  meta.properties[mName] = mValue;
}

}  // namespace tactile
