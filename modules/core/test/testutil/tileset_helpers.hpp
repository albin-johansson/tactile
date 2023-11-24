// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"
#include "tactile/core/tileset/tileset.hpp"

namespace tactile::test {

/**
 * \brief Returns a dummy tileset configuration useful for creating tilesets in tests.
 *
 * \return a tileset configuration.
 */
[[nodiscard]]
auto make_dummy_tileset_info() -> TilesetCreateInfo;

}  // namespace tactile::test
