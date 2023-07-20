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

#include "set_object_tag.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/objects/object_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

SetObjectTag::SetObjectTag(Registry* registry, const Entity object_entity, String tag)
    : mRegistry {registry},
      mObjectEntity {object_entity},
      mNewTag {std::move(tag)}
{
  TACTILE_ASSERT(sys::is_object_entity(*mRegistry, mObjectEntity));
}

void SetObjectTag::undo()
{
  auto& registry = *mRegistry;

  auto& object = registry.get<Object>(mObjectEntity);
  object.tag = mOldTag.value();

  mOldTag.reset();
}

void SetObjectTag::redo()
{
  auto& registry = *mRegistry;
  auto& object = registry.get<Object>(mObjectEntity);

  mOldTag = object.tag;
  object.tag = mNewTag;
}

auto SetObjectTag::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetObjectTag*>(cmd)) {
    if (mObjectEntity == other->mObjectEntity) {
      mNewTag = other->mNewTag;
      return true;
    }
  }

  return false;
}

auto SetObjectTag::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.update_object_tag;
}

}  // namespace tactile::cmd
