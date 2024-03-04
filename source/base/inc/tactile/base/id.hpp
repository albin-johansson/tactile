// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"

namespace tactile {

using LayerID = int32;

using ObjectID = int32;

/** Represents local tile identifiers. */
using TileIndex = int32;

/** Represents global tile identifiers. */
using TileID = int32;

/** The identifier used by empty tiles. */
inline constexpr TileID kEmptyTile = 0;

}  // namespace tactile
