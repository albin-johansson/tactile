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

#pragma once

#include <string>  // string

#include "core/common/uuid.hpp"
#include "core/layers/layer_type.hpp"

namespace tactile {

/// Event for creating a new layer.
struct AddLayerEvent final
{
  LayerType type {};
};

/// Event for removing a layer.
struct RemoveLayerEvent final
{
  UUID layer_id {};
};

/// Event for duplicating an existing layer.
struct DuplicateLayerEvent final
{
  UUID layer_id {};
};

/// Event for setting the active layer.
struct SelectLayerEvent final
{
  UUID layer_id {};
};

/// Event for moving a layer up in the render order.
struct MoveLayerUpEvent final
{
  UUID id {};
};

/// Event for moving a layer down in the render order.
struct MoveLayerDownEvent final
{
  UUID layer_id {};
};

/// Event for changing the opacity of a layer.
struct SetLayerOpacityEvent final
{
  UUID  layer_id {};
  float opacity {};
};

/// Event for changing the visibility of a layer.
struct SetLayerVisibleEvent final
{
  UUID layer_id {};
  bool visible {};
};

/// Event for opening the dialog to change a layer name.
struct OpenRenameLayerDialogEvent final
{
  UUID layer_id {};
};

/// Event for changing the name of a layer.
struct RenameLayerEvent final
{
  UUID        layer_id {};
  std::string name;
};

}  // namespace tactile
