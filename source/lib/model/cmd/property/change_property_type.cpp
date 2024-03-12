// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "change_property_type.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

ChangePropertyType::ChangePropertyType(Shared<Context> context,
                                       String name,
                                       const AttributeType type)
    : mContext {std::move(context)},
      mName {std::move(name)},
      mPropertyType {type}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void ChangePropertyType::undo()
{
  mContext->get_ctx().update_property(mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void ChangePropertyType::redo()
{
  auto& ctx = mContext->get_ctx();

  mPreviousValue = ctx.get_property(mName);
  ctx.update_property(mName, Attribute {mPropertyType});
}

auto ChangePropertyType::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.change_property_type;
}

}  // namespace tactile::cmd
