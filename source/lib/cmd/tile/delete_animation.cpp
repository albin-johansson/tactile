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

#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

DeleteAnimation::DeleteAnimation(const Entity tile_entity)
    : mTileEntity {tile_entity}
{
}

void DeleteAnimation::undo()
{
  auto& model = get_global_model();

  auto& animation = model.add<TileAnimation>(mTileEntity);
  animation = mOldTileAnimation.value();

  mOldTileAnimation.reset();
}

void DeleteAnimation::redo()
{
  auto& model = get_global_model();

  mOldTileAnimation = model.get<TileAnimation>(mTileEntity);
  model.remove<TileAnimation>(mTileEntity);
}

auto DeleteAnimation::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.delete_animation;
}

}  // namespace tactile::cmd
