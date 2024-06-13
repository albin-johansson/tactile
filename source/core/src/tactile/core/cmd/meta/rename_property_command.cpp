// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/rename_property_command.hpp"

#include <utility>  // move

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

RenamePropertyCommand::RenamePropertyCommand(IDocument* document,
                                             const EntityID context_entity,
                                             String old_name,
                                             String new_name)
  : mDocument {require_not_null(document, "null document")},
    mContextEntity {context_entity},
    mOldName {std::move(old_name)},
    mNewName {std::move(new_name)}
{}

void RenamePropertyCommand::undo()
{
  TACTILE_SET_LOG_SCOPE("RenamePropertyCommand");
  _rename_property(mNewName, mOldName);
}

void RenamePropertyCommand::redo()
{
  TACTILE_SET_LOG_SCOPE("RenamePropertyCommand");
  _rename_property(mOldName, mNewName);
}

auto RenamePropertyCommand::merge_with(const ICommand* cmd) -> bool
{
  const auto* that = dynamic_cast<const RenamePropertyCommand*>(cmd);
  if (!that) {
    return false;
  }

  const auto can_merge = (this->mDocument == that->mDocument) &&
                         (this->mContextEntity == that->mContextEntity) &&
                         (this->mNewName == that->mOldName);
  if (!can_merge) {
    return false;
  }

  this->mNewName = that->mNewName;

  return true;
}

void RenamePropertyCommand::_rename_property(const StringView from, String to)
{
  TACTILE_LOG_DEBUG("Renaming property '{}' to '{}'", from, to);

  auto& registry = mDocument->get_registry();
  auto& meta = registry.get<CMeta>(mContextEntity);

  TACTILE_ASSERT(exists_in(meta.properties, from));
  TACTILE_ASSERT(!exists_in(meta.properties, to));

  auto property = take_from(meta.properties, from).value();
  meta.properties.insert_or_assign(std::move(to), std::move(property));
}

}  // namespace tactile
