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

#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameLayerCmd::RenameLayerCmd(MapDocument* document,
                               const UUID&  layerId,
                               std::string  name)
    : mDocument{document}
    , mLayerId{layerId}
    , mName{std::move(name)}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void RenameLayerCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  layer.set_name(mPreviousName.value());
  mPreviousName.reset();
}

void RenameLayerCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  mPreviousName = layer.get_name();
  layer.set_name(mName);
}

auto RenameLayerCmd::get_name() const -> const char*
{
  return "Rename Layer";
}

}  // namespace tactile
