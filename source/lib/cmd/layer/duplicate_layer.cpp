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

#include "duplicate_layer.hpp"

#include "common/debug/assert.hpp"
#include "components/document.hpp"
#include "core/map.hpp"
#include "model/context.hpp"
#include "model/systems/group_layer_system.hpp"
#include "model/systems/layer_system.hpp"
#include "model/systems/map_system.hpp"
#include "model/systems/validation.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(const Entity map_entity, const Entity layer_entity)
    : mMapEntity {map_entity},
      mSourceLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(get_global_model(), map_entity));
}

void DuplicateLayer::undo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  sys::remove_layer_from_map(model, map, mNewLayerEntity);

  model.set_enabled(mNewLayerEntity, false);
}

void DuplicateLayer::redo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  mNewLayerParentEntity =
      sys::get_parent_layer(model, map.root_layer, mSourceLayerEntity);

  if (mNewLayerEntity != kNullEntity) {
    model.set_enabled(mNewLayerEntity, true);

    sys::attach_layer_to_map(model, map, mNewLayerEntity, mNewLayerParentEntity);
    sys::set_layer_local_index(model, map.root_layer, mNewLayerEntity, mNewIndex.value());
  }
  else {
    const auto layer_id = map.next_layer_id++;
    mNewLayerEntity =
        sys::duplicate_layer(model, map.root_layer, mSourceLayerEntity, layer_id);
    mNewIndex = sys::get_local_layer_index(model, root_layer, mNewLayerEntity);
  }
}

auto DuplicateLayer::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.duplicate_layer;
}

}  // namespace tactile::cmd