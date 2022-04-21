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

#include "stamp_tool_cmd.hpp"

#include <utility>  // move

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"

namespace tactile {

StampToolCmd::StampToolCmd(RegistryRef registry,
                           TileCache&& oldState,
                           TileCache&& newState)
    : ACommand{"Stamp Sequence"}
    , mRegistry{registry}
    , mLayer{sys::get_active_layer_id(registry).value()}
    , mOldState{std::move(oldState)}
    , mNewState{std::move(newState)}
{}

void StampToolCmd::undo()
{
  ApplySequence(mOldState);
}

void StampToolCmd::redo()
{
  ApplySequence(mNewState);
}

void StampToolCmd::ApplySequence(const TileCache& cache)
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  auto& layer = sys::checked_get<comp::TileLayer>(registry, entity);

  sys::set_tiles(layer, cache);
}

}  // namespace tactile
