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

#include "remove_layer.hpp"

#include "core/document/map_document.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(MapDocument* document, const UUID& layer_id)
    : mDocument {document}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }

  auto& map = mDocument->get_map();
  mLayer = map.get_layer(layer_id);
}

void RemoveLayer::undo()
{
  auto& map = mDocument->get_map();
  map.add_layer(mLayer, mLayer->get_parent());
  map.set_layer_index(mLayer->get_uuid(), mIndex.value());

  mDocument->get_contexts().add_context(mLayer);
  mIndex.reset();
}

void RemoveLayer::redo()
{
  auto&      map = mDocument->get_map();
  const auto id = mLayer->get_uuid();

  mIndex = map.local_layer_index(id);
  map.remove_layer(id);

  mDocument->get_contexts().erase(id);
}

auto RemoveLayer::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_layer;
}

}  // namespace tactile::cmd