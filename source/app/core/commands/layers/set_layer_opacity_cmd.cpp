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

#include "set_layer_opacity_cmd.hpp"

#include "core/common/ecs.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(RegistryRef registry,
                                       const LayerID id,
                                       const float opacity)
    : ACommand{"Set Layer Opacity"}
    , mRegistry{registry}
    , mLayerId{id}
    , mOpacity{opacity}
{}

void SetLayerOpacityCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  auto& layer = checked_get<comp::Layer>(registry, layerEntity);

  layer.opacity = mPreviousOpacity.value();
  mPreviousOpacity.reset();
}

void SetLayerOpacityCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  auto& layer = checked_get<comp::Layer>(registry, layerEntity);

  mPreviousOpacity = layer.opacity;
  layer.opacity = mOpacity;
}

auto SetLayerOpacityCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetLayerOpacityCmd&>(cmd);
    if (mLayerId == other.mLayerId) {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile