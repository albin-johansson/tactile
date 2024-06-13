// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/remove_property_command.hpp"

#include <utility>  // move

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

RemovePropertyCommand::RemovePropertyCommand(IDocument* document,
                                             const EntityID context_entity,
                                             String name)
  : mDocument {require_not_null(document, "null document")},
    mContextEntity {context_entity},
    mName {std::move(name)},
    mValue {kNone}
{}

void RemovePropertyCommand::undo()
{
  TACTILE_SET_LOG_SCOPE("RemovePropertyCommand");
  TACTILE_LOG_DEBUG("Restoring property '{}' to {}",
                    mName,
                    entity_to_string(mContextEntity));

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  meta.properties.insert_or_assign(mName, mValue.value());
}

void RemovePropertyCommand::redo()
{
  TACTILE_SET_LOG_SCOPE("RemovePropertyCommand");
  TACTILE_LOG_DEBUG("Removing property '{}' from {}",
                    mName,
                    entity_to_string(mContextEntity));

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  mValue = take_from(meta.properties, mName);
}

}  // namespace tactile
