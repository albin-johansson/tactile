// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "define_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

DefineComponent::DefineComponent(Shared<ComponentIndex> index, String name)
    : mIndex {std::move(index)},
      mName {std::move(name)}
{
  if (!mIndex) {
    throw Exception {"Invalid null component index!"};
  }
}

void DefineComponent::undo()
{
  const auto& component_def = mDef.value();
  mIndex->remove_comp(component_def.get_uuid());
}

void DefineComponent::redo()
{
  if (mDef.has_value()) {
    mIndex->restore_comp(*mDef);
  }
  else {
    const auto component_id = mIndex->define_comp(mName);
    mDef = mIndex->get_comp(component_id);
  }
}

auto DefineComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.define_comp;
}

}  // namespace tactile::cmd
