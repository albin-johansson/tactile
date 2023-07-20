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
#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/group_layer_ops.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(Registry* registry,
                               const Entity map_entity,
                               const Entity layer_entity)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mSourceLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(*mRegistry, mMapEntity));
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, mSourceLayerEntity));
}

void DuplicateLayer::undo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  sys::remove_layer_from_map(registry, map, mNewLayerEntity);

  registry.set_enabled(mNewLayerEntity, false);
}

void DuplicateLayer::redo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  const auto& root_layer = registry.get<GroupLayer>(map.root_layer);

  mNewLayerParentEntity = sys::get_parent_layer(registry, map, mSourceLayerEntity);

  if (mNewLayerEntity != kNullEntity) {
    registry.set_enabled(mNewLayerEntity, true);

    sys::attach_layer_to_map(registry, map, mNewLayerEntity, mNewLayerParentEntity);
    sys::set_layer_local_index(registry, map, mNewLayerEntity, mNewIndex.value());
  }
  else {
    mNewLayerEntity = sys::duplicate_layer(registry, mMapEntity, mSourceLayerEntity);
    mNewIndex = sys::get_local_layer_index(registry, root_layer, mNewLayerEntity);
  }
}

auto DuplicateLayer::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.duplicate_layer;
}

}  // namespace tactile::cmd