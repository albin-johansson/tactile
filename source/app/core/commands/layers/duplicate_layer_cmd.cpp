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

#include "core/common/ecs.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

DuplicateLayerCmd::DuplicateLayerCmd(MapDocument* map, const UUID& layerId)
    : ACommand{"Duplicate Layer"}
    , mMap{map}
    , mLayerId{layerId}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void DuplicateLayerCmd::undo()
{
  auto& registry = mMap->get_registry();

  const auto layerEntity = sys::find_context(registry, mNewLayerId.value());
  sys::remove_layer(registry, layerEntity);
}

void DuplicateLayerCmd::redo()
{
  auto& registry = mMap->get_registry();

  const auto sourceEntity = sys::find_context(registry, mLayerId);
  const auto entity = sys::duplicate_layer(registry, sourceEntity);

  if (!mNewLayerId) {
    const auto& context = checked_get<comp::AttributeContext>(registry, entity);
    mNewLayerId = context.id;
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(sys::find_context(registry, *mNewLayerId) == entt::null);
    auto& context = checked_get<comp::AttributeContext>(registry, entity);
    sys::set_context_id(context, *mNewLayerId);
  }
}

}  // namespace tactile