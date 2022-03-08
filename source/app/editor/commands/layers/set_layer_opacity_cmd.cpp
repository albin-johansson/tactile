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

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

set_layer_opacity_cmd::set_layer_opacity_cmd(registry_ref registry,
                                             const layer_id id,
                                             const float opacity)
    : command_base{"Set Layer Opacity"}
    , mRegistry{registry}
    , mLayerId{id}
    , mOpacity{opacity}
{}

void set_layer_opacity_cmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  layer.opacity = mPreviousOpacity.value();

  mPreviousOpacity.reset();
}

void set_layer_opacity_cmd::redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  mPreviousOpacity = layer.opacity;

  layer.opacity = mOpacity;
}

auto set_layer_opacity_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_layer_opacity_cmd&>(cmd);
    if (mLayerId == other.mLayerId) {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
