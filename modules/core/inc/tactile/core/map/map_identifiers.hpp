// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Tracks available identifiers in maps.
 */
struct MapIdentifiers final {
  LayerID next_layer_id {1};
  ObjectID next_object_id {1};
};

}  // namespace tactile
