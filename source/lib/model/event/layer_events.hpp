// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/layer_type.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Event for creating a new layer.
struct AddLayerEvent final {
  LayerType type {};
};

/// Event for removing a layer.
struct RemoveLayerEvent final {
  UUID layer_id {};
};

/// Event for duplicating an existing layer.
struct DuplicateLayerEvent final {
  UUID layer_id {};
};

/// Event for setting the active layer.
struct SelectLayerEvent final {
  UUID layer_id {};
};

/// Event for moving a layer up in the render order.
struct MoveLayerUpEvent final {
  UUID layer_id {};
};

/// Event for moving a layer down in the render order.
struct MoveLayerDownEvent final {
  UUID layer_id {};
};

/// Event for changing the opacity of a layer.
struct SetLayerOpacityEvent final {
  UUID layer_id {};
  float opacity {};
};

/// Event for changing the visibility of a layer.
struct SetLayerVisibleEvent final {
  UUID layer_id {};
  bool visible {};
};

/// Event for opening the dialog to change a layer name.
struct OpenRenameLayerDialogEvent final {
  UUID layer_id {};
};

/// Event for changing the name of a layer.
struct RenameLayerEvent final {
  UUID layer_id {};
  String name;
};

}  // namespace tactile
