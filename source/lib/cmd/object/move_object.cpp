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

#include "move_object.hpp"

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/objects/object_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

MoveObject::MoveObject(Registry* registry,
                       const Entity object_entity,
                       const Float2 old_position,
                       const Float2 new_position)
    : mRegistry {registry},
      mObjectEntity {object_entity},
      mOldPosition {old_position},
      mNewPosition {new_position}
{
  TACTILE_ASSERT(sys::is_object_entity(*mRegistry, mObjectEntity));
}

void MoveObject::undo()
{
  auto& registry = *mRegistry;

  auto& object = registry.get<Object>(mObjectEntity);
  object.position = mOldPosition;
}

void MoveObject::redo()
{
  auto& registry = *mRegistry;

  auto& object = registry.get<Object>(mObjectEntity);
  object.position = mNewPosition;
}

auto MoveObject::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.move_object;
}

}  // namespace tactile::cmd
