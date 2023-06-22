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

#include "set_object_visible.hpp"

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/objects/object_components.hpp"

namespace tactile::cmd {

SetObjectVisible::SetObjectVisible(Model* model,
                                   const Entity object_entity,
                                   const bool new_visibility)
    : mModel {model},
      mObjectEntity {object_entity},
      mNewVisibility {new_visibility}
{
  TACTILE_ASSERT(sys::is_object_entity(*mModel, mObjectEntity));
}

void SetObjectVisible::undo()
{
  auto& model = *mModel;

  auto& object = model.get<Object>(mObjectEntity);
  object.visible = mOldVisibility.value();

  mOldVisibility.reset();
}

void SetObjectVisible::redo()
{
  auto& model = *mModel;
  auto& object = model.get<Object>(mObjectEntity);

  mOldVisibility = static_cast<bool>(object.visible);
  object.visible = mNewVisibility;
}

auto SetObjectVisible::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return mNewVisibility ? strings.cmd.show_object : strings.cmd.hide_object;
}

}  // namespace tactile::cmd
