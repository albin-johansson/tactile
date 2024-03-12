// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "add_property.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddProperty::AddProperty(Shared<Context> context, String name, const AttributeType type)
    : mContext {std::move(context)},
      mName {std::move(name)},
      mType {type}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void AddProperty::undo()
{
  mContext->get_ctx().remove_property(mName);
}

void AddProperty::redo()
{
  mContext->get_ctx().add_property(mName, mType);
}

auto AddProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_property;
}

}  // namespace tactile::cmd
