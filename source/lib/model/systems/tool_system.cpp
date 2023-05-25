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

#include "tool_system.hpp"

#include "components/map.hpp"
#include "core/tileset.hpp"

namespace tactile::sys {

auto is_stamp_tool_randomizer_possible(const Model& model, const Entity map_entity)
    -> bool
{
  const auto& map = model.get<Map>(map_entity);

  if (map.active_tileset != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    return attached_tileset.is_single_tile_selected();
  }

  return false;
}

auto is_stamp_tool_randomizer_possible(const Model& model, const Map& map) -> bool
{
  if (map.active_tileset != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    return attached_tileset.is_single_tile_selected();
  }

  return false;
}

}  // namespace tactile::sys
