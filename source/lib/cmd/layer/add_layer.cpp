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

#include "add_layer.hpp"

#include "core/debug/assert.hpp"
#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

AddLayer::AddLayer(Registry* registry,
                   const Entity map_document_entity,
                   const LayerType type)
    : mRegistry {registry},
      mMapDocumentEntity {map_document_entity},
      mLayerType {type}
{
  TACTILE_ASSERT(sys::is_map_document_entity(*mRegistry, mMapDocumentEntity));
}

void AddLayer::undo()
{
  auto& registry = *mRegistry;

  const auto& map_document = registry.get<MapDocument>(mMapDocumentEntity);
  auto& map = registry.get<Map>(map_document.map);

  sys::remove_layer_from_map(registry, map, mLayerEntity);

  auto& document = registry.get<Document>(mMapDocumentEntity);
  if (document.active_context == mLayerEntity) {
    document.active_context = kNullEntity;
  }

  registry.set_enabled(mLayerEntity, false);
  mLayerWasAdded = false;
}

void AddLayer::redo()
{
  auto& registry = *mRegistry;
  const auto& map_document = registry.get<MapDocument>(mMapDocumentEntity);

  if (mLayerEntity == kNullEntity) {
    mLayerEntity = sys::add_new_layer_to_map(registry, map_document.map, mLayerType);
  }
  else {
    registry.set_enabled(mLayerEntity, true);

    auto& map = registry.get<Map>(map_document.map);
    sys::attach_layer_to_map(registry, map, mLayerEntity, mParentLayerEntity);
  }

  mLayerWasAdded = true;
}

void AddLayer::dispose()
{
  if (!mLayerWasAdded) {
    mRegistry->destroy(mLayerEntity);
  }
}

auto AddLayer::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.add_layer;
}

}  // namespace tactile::cmd
