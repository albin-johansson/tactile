// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/entity/entity.hpp>

#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct SelectObjectEvent final {
  UUID layer_id {};
  UUID object_id {};
};

/// Emitted after an object has been moved.
struct MoveObjectEvent final {
  UUID object_id {};  /// ID of the object to move.
  Float2 previous;    /// Previous position of the object.
  Float2 updated;     /// Updated position of the object.
};

/// Emitted when the visibility of an object should be changed.
struct SetObjectVisibleEvent final {
  UUID object_id {};  /// ID of the target object.
  bool visible {};    /// New visibility state.
};

/// Emitted when the tag of an object should be changed.
struct SetObjectTagEvent final {
  UUID object_id {};  /// ID of target object.
  String tag;         /// The new object tag.
};

/// Emitted when the name of an object should be changed.
struct SetObjectNameEvent final {
  UUID object_id {};  /// ID of target object.
  String name;        /// The new object name.
};

struct DuplicateObjectEvent final {
  UUID object_id {};  /// ID of the target object.
};

struct RemoveObjectEvent final {
  UUID object_id {};  /// ID of the target object.
};

struct SpawnObjectContextMenuEvent final {
  UUID object_id {};
};

}  // namespace tactile
