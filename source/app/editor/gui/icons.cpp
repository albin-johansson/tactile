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

#include "icons.hpp"

#include "misc/throw.hpp"

namespace tactile {

IconManager::IconManager(TextureManager& textures)
    : mTactileIcon{textures.load("resources/icon.png").value().id}
{}

auto get_icon(const LayerType type) -> const char*
{
  switch (type) {
    case LayerType::tile_layer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::object_layer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::group_layer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw_traced(tactile_error{"Failed to recognize layer type!"});
  }
}

}  // namespace tactile
