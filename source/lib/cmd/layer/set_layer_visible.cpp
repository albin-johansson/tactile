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

#include "set_layer_visible.hpp"

#include "components/layer.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

SetLayerVisible::SetLayerVisible(const Entity layer_entity, const bool visible)
    : mLayerEntity {layer_entity},
      mNewVisibility {visible}
{
}

void SetLayerVisible::undo()
{
  auto& model = get_global_model();
  auto& layer = model.get<Layer>(mLayerEntity);

  layer.visible = mOldVisibility.value();
  mOldVisibility.reset();
}

void SetLayerVisible::redo()
{
  auto& model = get_global_model();
  auto& layer = model.get<Layer>(mLayerEntity);

  mOldVisibility = static_cast<bool>(layer.visible);
  layer.visible = mNewVisibility;
}

auto SetLayerVisible::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return mNewVisibility ? strings.cmd.show_layer : strings.cmd.hide_layer;
}

}  // namespace tactile::cmd
