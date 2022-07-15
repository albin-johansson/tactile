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

#include "core/document/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

DuplicateLayerCmd::DuplicateLayerCmd(MapDocument* document, const UUID& layerId)
    : mDocument {document}
    , mLayerId {layerId}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }
}

void DuplicateLayerCmd::undo()
{
  auto&      map = mDocument->get_map();
  const auto id = mNewLayer->get_uuid();

  map.remove_layer(id);
  mDocument->get_contexts().erase(id);
}

void DuplicateLayerCmd::redo()
{
  auto& map = mDocument->get_map();

  if (mNewLayer) {
    map.add_layer(mNewLayer, mNewLayer->get_parent());
    map.set_layer_index(mNewLayer->get_uuid(), mNewIndex.value());
  }
  else {
    mNewLayer = map.duplicate_layer(mLayerId);
    mNewIndex = map.local_layer_index(mNewLayer->get_uuid());
  }

  mDocument->get_contexts().add_context(mNewLayer);
}

auto DuplicateLayerCmd::get_name() const -> const char*
{
  return "Duplicate Layer";
}

}  // namespace tactile