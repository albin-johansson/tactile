// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "update_property.hpp"

#include <utility>  // move

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

UpdateProperty::UpdateProperty(Shared<Context> context, String name, Attribute value)
    : mContext {std::move(context)},
      mName {std::move(name)},
      mNewValue {std::move(value)}
{
  if (!mContext) {
    throw Exception {"Invalid null context!"};
  }
}

void UpdateProperty::undo()
{
  mContext->get_ctx().update_property(mName, mOldValue.value());
  mOldValue.reset();
}

void UpdateProperty::redo()
{
  auto& ctx = mContext->get_ctx();
  mOldValue = ctx.get_property(mName);
  ctx.update_property(mName, mNewValue);
}

auto UpdateProperty::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateProperty*>(cmd)) {
    if (mContext->get_uuid() == other->mContext->get_uuid() && mName == other->mName) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateProperty::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.update_property;
}

}  // namespace tactile::cmd
