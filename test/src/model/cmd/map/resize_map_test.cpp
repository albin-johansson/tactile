/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "model/cmd/map/resize_map.hpp"

#include <doctest/doctest.h>

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::ResizeMap")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::ResizeMap(nullptr, TileExtent {1, 1}), Exception);
  }

  TEST_CASE("redo/undo")
  {
    const TileExtent old_extent {5, 7};
    const TileExtent new_extent {3, 9};

    auto map_document = test::MapBuilder::build()  //
                            .with_size(old_extent.rows, old_extent.cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::ResizeMap cmd {map, new_extent};

    cmd.redo();
    REQUIRE(new_extent == map->get_extent());

    cmd.undo();
    REQUIRE(old_extent == map->get_extent());
  }
}

}  // namespace tactile::test
