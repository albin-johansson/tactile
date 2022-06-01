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

#include "fix_tiles_in_map_cmd.hpp"

#include "core/common/ecs.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/panic.hpp"

namespace tactile {

FixTilesInMapCmd::FixTilesInMapCmd(MapDocument* map)
    : ACommand{"Fix Tiles In Map"}
    , mMap{map}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void FixTilesInMapCmd::undo()
{
  auto& registry = mMap->get_registry();

  for (const auto& [id, previous] : mResult) {
    const auto layerEntity = sys::find_context(registry, id);
    auto& layer = checked_get<comp::TileLayer>(registry, layerEntity);

    for (const auto& [pos, tile] : previous) {
      sys::set_tile(layer, pos, tile);
    }
  }

  mResult.clear();
}

void FixTilesInMapCmd::redo()
{
  auto& registry = mMap->get_registry();
  mResult = sys::fix_tiles_in_map(registry);
}

}  // namespace tactile
