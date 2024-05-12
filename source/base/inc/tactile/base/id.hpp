// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // hash

#include "tactile/base/int.hpp"
#include "tactile/base/util/hash.hpp"
#include "tactile/base/util/strong_type.hpp"

namespace tactile {

using LayerID = int32;

using ObjectID = int32;

/** Represents local tile identifiers. */
using TileIndex = int32;

/** Represents global tile identifiers. */
using TileID = int32;

/** Strong type for object reference identifiers. */
TACTILE_STRONG_TYPE(ObjectRef, int32);

/** Strong type for texture identifiers. */
TACTILE_STRONG_TYPE(TextureID, uint16);

/** The identifier used by empty tiles. */
inline constexpr TileID kEmptyTile = 0;

}  // namespace tactile

template <>
struct std::hash<tactile::TextureID> final
{
  [[nodiscard]]
  auto operator()(const tactile::TextureID id) const -> tactile::usize
  {
    return tactile::hash_combine(id.value);
  }
};
