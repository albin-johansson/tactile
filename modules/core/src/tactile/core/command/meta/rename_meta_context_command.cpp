// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_meta_context_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

RenameMetaContextCommand::RenameMetaContextCommand(IDocument* document,
                                                   const UUID& context_uuid,
                                                   String new_name)
  : mDocument {require_not_null(document, "null document")},
    mContextUuid {context_uuid},
    mNewName {std::move(new_name)}
{}

void RenameMetaContextCommand::undo()
{
  TACTILE_LOG_TRACE("[RenameMetaContextCommand::undo] context: {}, new name: {}",
                    fmt::streamed(mContextUuid),
                    mOldName.value());

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& metadata = context->meta();
    metadata.set_name(mOldName.value());
  }
  else {
    TACTILE_LOG_ERROR("[RenameMetaContextCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

void RenameMetaContextCommand::redo()
{
  TACTILE_LOG_TRACE("[RenameMetaContextCommand::redo] context: {}, new name: {}",
                    fmt::streamed(mContextUuid),
                    mNewName);

  if (auto* context = mDocument->find_context(mContextUuid)) {
    auto& metadata = context->meta();

    mOldName = metadata.name();
    metadata.set_name(mNewName);
  }
  else {
    TACTILE_LOG_ERROR("[RenameMetaContextCommand] Could not find context {}",
                      fmt::streamed(mContextUuid));
  }
}

auto RenameMetaContextCommand::merge_with(const ICommand* other) -> bool
{
  if (const auto* that = dynamic_cast<const RenameMetaContextCommand*>(other)) {
    if (this->mDocument == that->mDocument && this->mContextUuid == that->mContextUuid) {
      this->mNewName = that->mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
