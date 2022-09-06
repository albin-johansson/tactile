/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "rename_layer.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

RenameLayer::RenameLayer(Shared<Map> map, const UUID& layer_id, std::string name)
    : mMap {std::move(map)}
    , mLayerId {layer_id}
    , mNewName {std::move(name)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void RenameLayer::undo()
{
  auto& layer = mMap->view_layer(mLayerId);

  layer.set_name(mOldName.value());
  mOldName.reset();
}

void RenameLayer::redo()
{
  auto& layer = mMap->view_layer(mLayerId);

  mOldName = layer.get_name();
  layer.set_name(mNewName);
}

auto RenameLayer::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_layer;
}

}  // namespace tactile::cmd