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

#include "point_tool_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

PointToolCmd::PointToolCmd(RegistryRef registry, const float x, const float y)
    : ACommand{"Add Point"}
    , mRegistry{registry}
    , mLayerId{sys::get_active_layer_id(registry).value()}
    , mPointX{x}
    , mPointY{y}
{}

void PointToolCmd::undo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_object(registry, mObjectId.value());
}

void PointToolCmd::redo()
{
  auto& registry = mRegistry.get();
  if (mObjectId) {
    sys::restore_object(registry, mSnapshot.value());
  }
  else {
    mObjectId = sys::make_point_object(registry, mLayerId, mPointX, mPointY);
  }
}

}  // namespace tactile