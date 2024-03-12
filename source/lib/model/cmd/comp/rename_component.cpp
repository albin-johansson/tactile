// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameComponent::RenameComponent(Shared<ComponentIndex> index,
                                 const UUID& component_id,
                                 String name)
    : mComponentIndex {std::move(index)},
      mComponentId {component_id},
      mNewName {std::move(name)}
{
  if (!mComponentIndex) {
    throw Exception {"Invalid null component index!"};
  }
}

void RenameComponent::undo()
{
  mComponentIndex->rename_comp(mComponentId, mOldName.value());
  mOldName.reset();
}

void RenameComponent::redo()
{
  mOldName = mComponentIndex->get_comp(mComponentId).get_name();
  mComponentIndex->rename_comp(mComponentId, mNewName);
}

auto RenameComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_comp;
}

}  // namespace tactile::cmd
