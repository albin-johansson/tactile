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

#include "set_layer_visibility_cmd.hpp"

#include "core/common/ecs.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

SetLayerVisibilityCmd::SetLayerVisibilityCmd(RegistryRef registry,
                                             const LayerID id,
                                             const bool visible)
    : ACommand{"Set Layer Visibility"}
    , mRegistry{registry}
    , mLayerId{id}
    , mVisible{visible}
{}

void SetLayerVisibilityCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  auto& layer = checked_get<comp::Layer>(registry, layerEntity);

  layer.visible = mPreviousVisibility.value();
  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  auto& layer = checked_get<comp::Layer>(registry, layerEntity);

  mPreviousVisibility = layer.visible;
  layer.visible = mVisible;
}

}  // namespace tactile
