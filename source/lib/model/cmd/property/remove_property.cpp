// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_property.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveProperty::RemoveProperty(Shared<Context> context, String name)
    : mContext {std::move(context)},
      mName {std::move(name)}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void RemoveProperty::undo()
{
  mContext->get_ctx().add_property(mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemoveProperty::redo()
{
  auto& ctx = mContext->get_ctx();
  mPreviousValue = ctx.get_property(mName);
  ctx.remove_property(mName);
}

auto RemoveProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_property;
}

}  // namespace tactile::cmd
