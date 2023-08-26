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

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(MapDocument* document, const UUID& layer_id)
    : mDocument {document}
{
  if (!mDocument) {
    throw Error {"Invalid null map document!"};
  }

  mLayer = mDocument->get_map().get_invisible_root().find_shared_layer(layer_id);
}

void RemoveLayer::undo()
{
  auto& map = mDocument->get_map();
  map.insert_layer(mLayer, mIndex.value());

  mDocument->get_contexts().add_context(mLayer);
  mIndex.reset();
}

void RemoveLayer::redo()
{
  auto& map = mDocument->get_map();
  const auto layer_id = mLayer->get_uuid();

  mIndex = map.get_invisible_root().local_layer_index(layer_id);
  map.remove_layer(layer_id);

  mDocument->get_contexts().remove_context(layer_id);
}

auto RemoveLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_layer;
}

}  // namespace tactile::cmd
