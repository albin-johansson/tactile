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

#include "core/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

SetLayerVisible::SetLayerVisible(Registry* registry,
                                 const Entity layer_entity,
                                 const bool new_visibility)
    : mRegistry {registry},
      mLayerEntity {layer_entity},
      mNewVisibility {new_visibility}
{
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, mLayerEntity));
}

void SetLayerVisible::undo()
{
  auto& registry = *mRegistry;
  auto& layer = registry.get<Layer>(mLayerEntity);

  layer.visible = mOldVisibility.value();
  mOldVisibility.reset();
}

void SetLayerVisible::redo()
{
  auto& registry = *mRegistry;
  auto& layer = registry.get<Layer>(mLayerEntity);

  mOldVisibility = static_cast<bool>(layer.visible);
  layer.visible = mNewVisibility;
}

auto SetLayerVisible::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return mNewVisibility ? strings.cmd.show_layer : strings.cmd.hide_layer;
}

}  // namespace tactile::cmd
