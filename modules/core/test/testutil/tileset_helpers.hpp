// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/tileset/tileset.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core::test {

/**
 * Returns a dummy tileset configuration useful for creating tilesets in tests.
 *
 * \return
 *    A tileset configuration.
 */
[[nodiscard]]
auto make_dummy_tileset_info() -> TilesetCreateInfo;

}  // namespace tactile::core::test
