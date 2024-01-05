// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/hash.hpp"
#include "tactile/foundation/misc/strong_type.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/** Strong type for local tile identifiers. */
TACTILE_STRONG_TYPE(TileIndex, int32);

TACTILE_STRONG_TYPE_ADD_BINARY_OP(TileIndex, +);
TACTILE_STRONG_TYPE_ADD_BINARY_OP(TileIndex, -);

/** Strong type for global tile identifiers. */
TACTILE_STRONG_TYPE(TileID, int32);

TACTILE_STRONG_TYPE_ADD_BINARY_OP(TileID, +);
TACTILE_STRONG_TYPE_ADD_BINARY_OP(TileID, -);

/** Strong type for layer identifiers. */
TACTILE_STRONG_TYPE(LayerID, int32);

/** Strong type for object identifiers. */
TACTILE_STRONG_TYPE(ObjectID, int32);

/** Strong type for object references. */
TACTILE_STRONG_TYPE(ObjectRef, int32);

/**
 * The identifier used by empty tiles.
 */
inline constexpr TileID kEmptyTile {0};

}  // namespace tactile

TACTILE_IMPLEMENT_HASH(tactile::TileIndex, value.value);

TACTILE_IMPLEMENT_HASH(tactile::TileID, value.value);

TACTILE_IMPLEMENT_HASH(tactile::LayerID, value.value);

TACTILE_IMPLEMENT_HASH(tactile::ObjectID, value.value);

TACTILE_IMPLEMENT_HASH(tactile::ObjectRef, value.value);
