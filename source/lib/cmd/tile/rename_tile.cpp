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

#include "rename_tile.hpp"

#include <utility>  // move

#include "core/context.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"

namespace tactile::cmd {

RenameTile::RenameTile(const Entity tile_entity, String new_name)
    : mTileEntity {tile_entity},
      mNewName {std::move(new_name)}
{
}

void RenameTile::undo()
{
  auto& model = get_model();

  auto& tile_context = model.get<Context>(mTileEntity);
  tile_context.name = mOldName.value();

  mOldName.reset();
}

void RenameTile::redo()
{
  auto& model = get_model();
  auto& tile_context = model.get<Context>(mTileEntity);

  mOldName = tile_context.name;
  tile_context.name = mNewName;
}

auto RenameTile::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_tile;
}

}  // namespace tactile::cmd
