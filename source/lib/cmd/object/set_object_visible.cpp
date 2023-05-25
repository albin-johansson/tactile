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

#include "core/object.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

SetObjectVisible::SetObjectVisible(const Entity object_entity, const bool visible)
    : mObjectEntity {object_entity},
      mNewVisibility {visible}
{
}

void SetObjectVisible::undo()
{
  auto& model = get_global_model();
  auto& object = model.get<Object>(mObjectEntity);

  object.visible = mOldVisibility.value();
  mOldVisibility.reset();
}

void SetObjectVisible::redo()
{
  auto& model = get_global_model();
  auto& object = model.get<Object>(mObjectEntity);

  mOldVisibility = static_cast<bool>(object.visible);
  object.visible = mNewVisibility;
}

auto SetObjectVisible::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return mNewVisibility ? strings.cmd.show_object : strings.cmd.hide_object;
}

}  // namespace tactile::cmd
