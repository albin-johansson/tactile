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

#include "duplicate_layer_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

DuplicateLayerCmd::DuplicateLayerCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Duplicate Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void DuplicateLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mNewLayerId.value());
  sys::remove_layer(registry, entity);
}

void DuplicateLayerCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto sourceEntity = sys::find_layer(registry, mLayerId);
  const auto entity = sys::duplicate_layer(registry, sourceEntity);

  if (!mNewLayerId) {
    const auto& layer = sys::checked_get<comp::Layer>(registry, entity);
    mNewLayerId = layer.id;
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(sys::find_layer(registry, *mNewLayerId) == entt::null);
    auto& layer = sys::checked_get<comp::Layer>(registry, entity);
    layer.id = *mNewLayerId;
  }
}

}  // namespace tactile