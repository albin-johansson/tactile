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

#include "duplicate_layer.hpp"

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"
#include "model/document/map_document.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(MapDocument* document, const UUID& layer_id)
    : mDocument {document},
      mLayerId {layer_id}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }
}

void DuplicateLayer::undo()
{
  auto& map = mDocument->get_map();
  const auto id = mNewLayer->uuid();

  map.remove_layer(id);
  mDocument->get_contexts().erase(id);
}

void DuplicateLayer::redo()
{
  auto& map = mDocument->get_map();
  auto& root = map.invisible_root();

  if (mNewLayer) {
    map.add_layer(mNewLayer, mNewLayer->parent());
    root.set_index(mNewLayer->uuid(), mNewIndex.value());
  }
  else {
    mNewLayer = map.duplicate_layer(mLayerId);
    mNewIndex = root.local_index(mNewLayer->uuid());
  }

  mDocument->get_contexts().add_context(mNewLayer);
}

auto DuplicateLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.duplicate_layer;
}

}  // namespace tactile::cmd