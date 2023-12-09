// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_meta_context_command.hpp"

#include <utility>  // move

#include "tactile/core/context/meta_context.hpp"
#include "tactile/core/context/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"

namespace tactile {

RenameMetaContextCommand::RenameMetaContextCommand(IMetaContext* context, String new_name)
  : mContext {require_not_null(context, "invalid null context")},
    mNewName {std::move(new_name)}
{}

void RenameMetaContextCommand::undo()
{
  auto& metadata = mContext->get_meta();
  metadata.set_name(mOldName.value());
}

void RenameMetaContextCommand::redo()
{
  auto& metadata = mContext->get_meta();

  mOldName = metadata.get_name();
  metadata.set_name(mNewName);
}

auto RenameMetaContextCommand::merge_with(const ICommand* other) -> bool
{
  if (const auto* that = dynamic_cast<const RenameMetaContextCommand*>(other)) {
    if (this->mContext->get_meta().get_uuid() == that->mContext->get_meta().get_uuid()) {
      this->mNewName = that->mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
