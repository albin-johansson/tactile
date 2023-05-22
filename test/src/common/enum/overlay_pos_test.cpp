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

#include "common/enum/overlay_pos.hpp"

#include <utility>  // to_underlying

#include <doctest/doctest.h>

#include <settings.pb.h>

namespace tactile::test {

TEST_SUITE("Overlay position enum")
{
  TEST_CASE("Enumerators should match corresponding proto enumerators")
  {
    REQUIRE(std::to_underlying(OverlayPos::TopLeft) == proto::OVERLAY_POS_TOP_LEFT);
    REQUIRE(std::to_underlying(OverlayPos::TopRight) == proto::OVERLAY_POS_TOP_RIGHT);
    REQUIRE(std::to_underlying(OverlayPos::BottomRight) ==
            proto::OVERLAY_POS_BOTTOM_RIGHT);
    REQUIRE(std::to_underlying(OverlayPos::BottomLeft) == proto::OVERLAY_POS_BOTTOM_LEFT);
  }
}

}  // namespace tactile::test
