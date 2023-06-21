/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <doctest/doctest.h>

#include "model/layers/layer_factory.hpp"
#include "model/layers/tile_layers.hpp"
#include "model/model.hpp"
#include "model/systems/layer_system.hpp"

using namespace tactile;

TEST_SUITE("TileLayerSystem")
{
  TEST_CASE("flood_tiles")
  {
    Model model;

    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    const auto layer_entity = sys::create_tile_layer(model, 1, TileExtent {5, 5});
    auto& layer = model.get<TileLayer>(layer_entity);

    // 0  0  0  0  0
    // 0  0  0  1  1
    // 0  0  0  1  0
    // 0  1  1  1  0
    // 1  0  0  0  0
    sys::set_tile(layer, {1, 3}, 1);
    sys::set_tile(layer, {1, 4}, 1);
    sys::set_tile(layer, {2, 3}, 1);
    sys::set_tile(layer, {3, 1}, 1);
    sys::set_tile(layer, {3, 2}, 1);
    sys::set_tile(layer, {3, 3}, 1);
    sys::set_tile(layer, {4, 0}, 1);

    // 0  0  0  0  0
    // 0  0  0  1  1
    // 0  0  0  1  2
    // 0  1  1  1  2
    // 1  2  2  2  2
    sys::flood_tiles(layer, {4, 1}, 2);

    REQUIRE(0 == sys::tile_at(layer, {0, 0}));
    REQUIRE(0 == sys::tile_at(layer, {0, 1}));
    REQUIRE(0 == sys::tile_at(layer, {0, 2}));
    REQUIRE(0 == sys::tile_at(layer, {0, 3}));
    REQUIRE(0 == sys::tile_at(layer, {0, 4}));

    REQUIRE(0 == sys::tile_at(layer, {1, 0}));
    REQUIRE(0 == sys::tile_at(layer, {1, 1}));
    REQUIRE(0 == sys::tile_at(layer, {1, 2}));
    REQUIRE(1 == sys::tile_at(layer, {1, 3}));
    REQUIRE(1 == sys::tile_at(layer, {1, 4}));

    REQUIRE(0 == sys::tile_at(layer, {2, 0}));
    REQUIRE(0 == sys::tile_at(layer, {2, 1}));
    REQUIRE(0 == sys::tile_at(layer, {2, 2}));
    REQUIRE(1 == sys::tile_at(layer, {2, 3}));
    REQUIRE(2 == sys::tile_at(layer, {2, 4}));

    REQUIRE(0 == sys::tile_at(layer, {3, 0}));
    REQUIRE(1 == sys::tile_at(layer, {3, 1}));
    REQUIRE(1 == sys::tile_at(layer, {3, 2}));
    REQUIRE(1 == sys::tile_at(layer, {3, 3}));
    REQUIRE(2 == sys::tile_at(layer, {3, 4}));

    REQUIRE(1 == sys::tile_at(layer, {4, 0}));
    REQUIRE(2 == sys::tile_at(layer, {4, 1}));
    REQUIRE(2 == sys::tile_at(layer, {4, 2}));
    REQUIRE(2 == sys::tile_at(layer, {4, 3}));
    REQUIRE(2 == sys::tile_at(layer, {4, 4}));

    // 0  0  0  0  0
    // 0  0  0  3  3
    // 0  0  0  3  2
    // 0  3  3  3  2
    // 1  2  2  2  2
    sys::flood_tiles(layer, {3, 1}, 3);

    REQUIRE(0 == sys::tile_at(layer, {0, 0}));
    REQUIRE(0 == sys::tile_at(layer, {0, 1}));
    REQUIRE(0 == sys::tile_at(layer, {0, 2}));
    REQUIRE(0 == sys::tile_at(layer, {0, 3}));
    REQUIRE(0 == sys::tile_at(layer, {0, 4}));

    REQUIRE(0 == sys::tile_at(layer, {1, 0}));
    REQUIRE(0 == sys::tile_at(layer, {1, 1}));
    REQUIRE(0 == sys::tile_at(layer, {1, 2}));
    REQUIRE(3 == sys::tile_at(layer, {1, 3}));
    REQUIRE(3 == sys::tile_at(layer, {1, 4}));

    REQUIRE(0 == sys::tile_at(layer, {2, 0}));
    REQUIRE(0 == sys::tile_at(layer, {2, 1}));
    REQUIRE(0 == sys::tile_at(layer, {2, 2}));
    REQUIRE(3 == sys::tile_at(layer, {2, 3}));
    REQUIRE(2 == sys::tile_at(layer, {2, 4}));

    REQUIRE(0 == sys::tile_at(layer, {3, 0}));
    REQUIRE(3 == sys::tile_at(layer, {3, 1}));
    REQUIRE(3 == sys::tile_at(layer, {3, 2}));
    REQUIRE(3 == sys::tile_at(layer, {3, 3}));
    REQUIRE(2 == sys::tile_at(layer, {3, 4}));

    REQUIRE(1 == sys::tile_at(layer, {4, 0}));
    REQUIRE(2 == sys::tile_at(layer, {4, 1}));
    REQUIRE(2 == sys::tile_at(layer, {4, 2}));
    REQUIRE(2 == sys::tile_at(layer, {4, 3}));
    REQUIRE(2 == sys::tile_at(layer, {4, 4}));
  }
}