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

#include "delete_animation.hpp"

#include "core/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

DeleteAnimation::DeleteAnimation(Registry* registry, const Entity tile_entity)
    : mRegistry {registry},
      mTileEntity {tile_entity}
{
  TACTILE_ASSERT(sys::is_tile_entity(*mRegistry, tile_entity));
}

void DeleteAnimation::undo()
{
  auto& registry = *mRegistry;

  auto& animation = registry.add<TileAnimation>(mTileEntity);
  animation = mOldTileAnimation.value();

  mOldTileAnimation.reset();
}

void DeleteAnimation::redo()
{
  auto& registry = *mRegistry;

  mOldTileAnimation = registry.get<TileAnimation>(mTileEntity);
  registry.remove<TileAnimation>(mTileEntity);
}

auto DeleteAnimation::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.delete_animation;
}

}  // namespace tactile::cmd
