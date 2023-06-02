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

#include "common/debug/assert.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/layer_system.hpp"
#include "model/systems/map_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(Model* model,
                         const Entity map_document_entity,
                         const Entity layer_entity)
    : mModel {model},
      mMapDocumentEntity {map_document_entity},
      mLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_document_entity(*mModel, mMapDocumentEntity));
  TACTILE_ASSERT(sys::is_layer_entity(*mModel, mLayerEntity));
}

void RemoveLayer::undo()
{
  auto& model = *mModel;

  const auto& map_document = model.get<MapDocument>(mMapDocumentEntity);
  auto& map = model.get<Map>(map_document.map);

  sys::attach_layer_to_map(model, map, mLayerEntity, mParentLayerEntity);
  // TODO sys::set_layer_index(model, map_document.map, mLayerEntity, mIndex.value());

  mIndex.reset();
}

void RemoveLayer::redo()
{
  auto& model = *mModel;

  const auto& map_document = model.get<MapDocument>(mMapDocumentEntity);
  auto& map = model.get<Map>(map_document.map);

  mParentLayerEntity = sys::get_parent_layer(model, map.root_layer, mLayerEntity);
  // TODO mIndex = sys::get_layer_local_index(model, map.root_layer, mLayerEntity);

  sys::remove_layer_from_map(model, map, mLayerEntity);
}

void RemoveLayer::dispose()
{
  // TODO
  //  auto& model = *mModel;
  //  model.destroy(m)
}

auto RemoveLayer::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.remove_layer;
}

}  // namespace tactile::cmd
