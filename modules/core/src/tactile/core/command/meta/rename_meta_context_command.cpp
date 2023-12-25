// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_meta_context_command.hpp"

#include <utility>  // move

#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"

namespace tactile {

RenameMetaContextCommand::RenameMetaContextCommand(IMetaContext* context, String new_name)
  : mContext {require_not_null(context, "invalid null context")},
    mNewName {std::move(new_name)}
{}

void RenameMetaContextCommand::undo()
{
}

void RenameMetaContextCommand::redo()
{

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
