// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/map/map_orientation.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * An event for opening the dialog for creating new map documents.
 */
struct OpenCreateMapDocumentDialogEvent final {};

/**
 * An event for opening the dialog for selecting existing documents to open.
 */
struct OpenSelectDocumentDialogEvent final {};

/**
 * An event for creating a new map document.
 */
struct CreateMapDocumentEvent final {
  MatrixExtent extent;
  Int2 tile_size;
  MapOrientation orientation;
};

}  // namespace tactile::core
