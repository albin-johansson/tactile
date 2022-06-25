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

#include "set_layer_visibility_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetLayerVisibilityCmd::SetLayerVisibilityCmd(MapDocument* document,
                                             const UUID&  layerId,
                                             const bool   visible)
    : ACommand{"Set Layer Visibility"}
    , mDocument{document}
    , mLayerId{layerId}
    , mVisible{visible}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void SetLayerVisibilityCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  layer.set_visible(mPreviousVisibility.value());
  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  mPreviousVisibility = layer.is_visible();
  layer.set_visible(mVisible);
}

}  // namespace tactile
