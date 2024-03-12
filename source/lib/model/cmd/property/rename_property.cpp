// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_property.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameProperty::RenameProperty(Shared<Context> context, String old_name, String new_name)
    : mContext {std::move(context)},
      mOldName {std::move(old_name)},
      mNewName {std::move(new_name)}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void RenameProperty::undo()
{
  mContext->get_ctx().rename_property(mNewName, mOldName);
}

void RenameProperty::redo()
{
  mContext->get_ctx().rename_property(mOldName, mNewName);
}

auto RenameProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_property;
}

}  // namespace tactile::cmd
