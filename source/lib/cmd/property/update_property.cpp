/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "update_property.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

UpdateProperty::UpdateProperty(Model* model,
                               const Entity context_entity,
                               String name,
                               Attribute new_value)
    : mModel {model},
      mContextEntity {context_entity},
      mName {std::move(name)},
      mNewValue {std::move(new_value)}
{
  TACTILE_ASSERT(sys::is_context_entity(*mModel, mContextEntity));
}

void UpdateProperty::undo()
{
  auto& model = *mModel;

  auto& context = model.get<Context>(mContextEntity);
  context.props[mName] = mOldValue.value();

  mUpdatedVectorComponentIndex.reset();
  mOldValue.reset();
}

void UpdateProperty::redo()
{
  auto& model = *mModel;
  auto& context = model.get<Context>(mContextEntity);

  mOldValue = lookup_in(context.props, mName);

  if (mNewValue.is_any_vector()) {
    mUpdatedVectorComponentIndex =
        get_first_different_vector_dimension(mNewValue, *mOldValue);
  }

  context.props[mName] = mNewValue;
}

auto UpdateProperty::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateProperty*>(cmd)) {
    if (mContextEntity == other->mContextEntity && mName == other->mName) {
      // We don't want to merge updates to different components of vector properties
      if (mNewValue.is_any_vector() &&
          mNewValue.get_type() == other->mNewValue.get_type() &&
          mUpdatedVectorComponentIndex != other->mUpdatedVectorComponentIndex) {
        return false;
      }

      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateProperty::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.update_property;
}

}  // namespace tactile::cmd
