// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <cstdint>     // int32_t, uint16_t
#include <functional>  // hash

#include "tactile/base/util/hash.hpp"
#include "tactile/base/util/strong_type.hpp"

namespace tactile {

using LayerID = std::int32_t;

using ObjectID = std::int32_t;

/** Represents local tile identifiers. */
using TileIndex = std::int32_t;

/** Represents global tile identifiers. */
using TileID = std::int32_t;

/** Strong type for object reference identifiers. */
TACTILE_STRONG_TYPE(ObjectRef, std::int32_t);

/** Strong type for texture identifiers. */
TACTILE_STRONG_TYPE(TextureID, std::uint16_t);

/** The identifier used by empty tiles. */
inline constexpr TileID kEmptyTile = 0;

}  // namespace tactile

template <>
struct std::hash<tactile::TextureID> final
{
  [[nodiscard]]
  auto operator()(const tactile::TextureID id) const -> std::size_t
  {
    return tactile::hash_combine(id.value);
  }
};
