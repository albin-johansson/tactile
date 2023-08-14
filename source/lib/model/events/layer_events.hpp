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

#pragma once

#include "core/ecs/entity.hpp"
#include "tactile/core/containers/string.hpp"
#include "core/enums/layer_type.hpp"

namespace tactile {

/// Event for creating a new layer.
struct CreateLayerEvent final {
  LayerType type {};
};

/// Event for removing a layer.
struct RemoveLayerEvent final {
  Entity layer {kNullEntity};
};

/// Event for duplicating an existing layer.
struct DuplicateLayerEvent final {
  Entity layer {kNullEntity};
};

/// Event for changing the name of a layer.
struct RenameLayerEvent final {
  Entity layer {kNullEntity};
  String name;
};

/// Event for setting the active layer.
struct SelectLayerEvent final {
  Entity layer {kNullEntity};
};

/// Event for moving a layer up in the render order.
struct MoveLayerUpEvent final {
  Entity layer {kNullEntity};
};

/// Event for moving a layer down in the render order.
struct MoveLayerDownEvent final {
  Entity layer {kNullEntity};
};

/// Event for changing the opacity of a layer.
struct SetLayerOpacityEvent final {
  Entity layer {kNullEntity};
  float opacity {};
};

/// Event for changing the visibility of a layer.
struct SetLayerVisibleEvent final {
  Entity layer {kNullEntity};
  bool visible {};
};

/// Event for opening the dialog to change a layer name.
struct ShowRenameLayerDialogEvent final {
  Entity layer {kNullEntity};
};

}  // namespace tactile
