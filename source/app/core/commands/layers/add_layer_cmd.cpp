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

#include "add_layer_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddLayerCmd::AddLayerCmd(MapDocument* document, const LayerType type)
    : mDocument {document}
    , mLayerType {type}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }
}

void AddLayerCmd::undo()
{
  auto&      map = mDocument->get_map();
  const auto id = mLayer->get_uuid();
  map.remove_layer(id);
  mDocument->unregister_context(id);
}

void AddLayerCmd::redo()
{
  auto& map = mDocument->get_map();

  if (mLayer) {
    map.add_layer(mLayer, mLayer->get_parent());
  }
  else {
    const auto activeLayerId =
        map.is_active_layer(LayerType::GroupLayer) ? map.active_layer_id() : nothing;
    Maybe<UUID> id;

    switch (mLayerType) {
      case LayerType::TileLayer: {
        id = map.add_tile_layer(activeLayerId);
        break;
      }
      case LayerType::ObjectLayer: {
        id = map.add_object_layer(activeLayerId);
        break;
      }
      case LayerType::GroupLayer: {
        id = map.add_group_layer(activeLayerId);
        break;
      }
      default:
        throw TactileError {"Invalid layer type!"};
    }

    mLayer = map.get_layer(id.value());
  }

  mDocument->register_context(mLayer);
}

auto AddLayerCmd::get_name() const -> const char*
{
  return "Add Layer";
}

}  // namespace tactile
