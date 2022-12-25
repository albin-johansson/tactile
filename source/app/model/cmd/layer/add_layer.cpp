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

#include "add_layer.hpp"

#include "core/layer/group_layer.hpp"
#include "debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"

namespace tactile::cmd {

AddLayer::AddLayer(MapDocument* document, const LayerType type)
    : mDocument {document},
      mLayerType {type}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }
}

void AddLayer::undo()
{
  auto& map = mDocument->get_map();
  const auto id = mLayer->get_uuid();
  map.remove_layer(id);
  mDocument->get_contexts().erase(id);
}

void AddLayer::redo()
{
  auto& map = mDocument->get_map();

  if (mLayer) {
    map.add_layer(mLayer, mLayer->get_parent());
  }
  else {
    const auto active_layer_id = map.is_active_layer(LayerType::GroupLayer)
                                     ? map.active_layer_id()  //
                                     : nothing;

    Maybe<UUID> id;
    switch (mLayerType) {
      case LayerType::TileLayer: {
        id = map.add_tile_layer(active_layer_id);
        break;
      }
      case LayerType::ObjectLayer: {
        id = map.add_object_layer(active_layer_id);
        break;
      }
      case LayerType::GroupLayer: {
        id = map.add_group_layer(active_layer_id);
        break;
      }
      default:
        throw TactileError {"Invalid layer type!"};
    }

    mLayer = map.invisible_root().find_shared_layer(id.value());
  }

  mDocument->get_contexts().add_context(mLayer);
}

auto AddLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_layer;
}

}  // namespace tactile::cmd
