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

#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/context.hpp"
#include "model/document.hpp"
#include "model/systems/layer_system.hpp"
#include "model/systems/map_system.hpp"

namespace tactile::cmd {

AddLayer::AddLayer(const Entity map_document_entity, const LayerType type)
    : mMapDocumentEntity {map_document_entity},
      mLayerType {type}
{
}

void AddLayer::undo()
{
  auto& model = get_global_model();

  const auto& map_document = model.get<MapDocument>(mMapDocumentEntity);
  auto& map = model.get<Map>(map_document.map);

  sys::remove_layer_from_map(model, map, mLayerEntity);

  auto& document = model.get<Document>(mMapDocumentEntity);
  if (document.active_context == mLayerEntity) {
    document.active_context = kNullEntity;
  }

  model.set_enabled(mLayerEntity, false);
  mLayerWasAdded = false;
}

void AddLayer::redo()
{
  auto& model = get_global_model();

  const auto& map_document = model.get<MapDocument>(mMapDocumentEntity);
  auto& map = model.get<Map>(map_document.map);

  if (mLayerEntity == kNullEntity) {
    mLayerEntity = sys::add_new_layer_to_map(model, map, mLayerType);
  }
  else {
    sys::attach_layer_to_map(model, map, mLayerEntity, mParentLayerEntity);
  }

  model.set_enabled(mLayerEntity, true);
  mLayerWasAdded = true;
}

void AddLayer::dispose()
{
  // The layer entity can only be destroyed if we're sure it's not used elsewhere.
  if (!mLayerWasAdded) {
    auto& model = get_global_model();
    model.destroy(mLayerEntity);
  }
}

auto AddLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_layer;
}

}  // namespace tactile::cmd
