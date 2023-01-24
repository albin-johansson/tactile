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

#include "fix_map_tiles.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

FixMapTiles::FixMapTiles(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map"};
  }
}

void FixMapTiles::undo()
{
  auto& root_layer = mMap->get_invisible_root();

  for (const auto& [layer_id, invalid_positions]: mInvalidLayerTiles) {
    auto& layer = root_layer.get_tile_layer(layer_id);

    for (const auto& [position, tile_id]: invalid_positions) {
      layer.set_tile(position, tile_id);
    }
  }

  mInvalidLayerTiles.clear();
}

void FixMapTiles::redo()
{
  mInvalidLayerTiles = mMap->fix_tiles();
}

auto FixMapTiles::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.fix_map_tiles;
}

}  // namespace tactile::cmd
