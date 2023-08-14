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

#include "remove_layer.hpp"

#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "tactile/core/common/assert.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(Registry* registry,
                         const Entity map_document_entity,
                         const Entity layer_entity)
    : mRegistry {registry},
      mMapDocumentEntity {map_document_entity},
      mLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_document_entity(*mRegistry, mMapDocumentEntity));
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, mLayerEntity));
}

void RemoveLayer::undo()
{
  auto& registry = *mRegistry;

  const auto& map_document = registry.get<MapDocument>(mMapDocumentEntity);
  auto& map = registry.get<Map>(map_document.map);

  sys::attach_layer_to_map(registry, map, mLayerEntity, mParentLayerEntity);
  // TODO sys::set_layer_index(registry, map_document.map, mLayerEntity, mIndex.value());

  mIndex.reset();
}

void RemoveLayer::redo()
{
  auto& registry = *mRegistry;

  const auto& map_document = registry.get<MapDocument>(mMapDocumentEntity);
  auto& map = registry.get<Map>(map_document.map);

  mParentLayerEntity = sys::get_parent_layer(registry, map, mLayerEntity);
  // TODO mIndex = sys::get_layer_local_index(registry, map.root_layer, mLayerEntity);

  sys::remove_layer_from_map(registry, map, mLayerEntity);
}

void RemoveLayer::dispose()
{
  // TODO
  //  auto& registry = *mRegistry;
  //  registry.destroy(m)
}

auto RemoveLayer::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.remove_layer;
}

}  // namespace tactile::cmd
