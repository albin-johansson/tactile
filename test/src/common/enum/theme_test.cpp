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

#include "common/enum/theme.hpp"

#include <utility>  // to_underlying

#include <doctest/doctest.h>

#include <settings.pb.h>

using namespace tactile;

TEST_SUITE("Theme")
{
  TEST_CASE("Enumerators should match corresponding proto enumerators")
  {
    REQUIRE(std::to_underlying(Theme::DearDark) == proto::THEME_DEAR_DARK);
    REQUIRE(std::to_underlying(Theme::DearLight) == proto::THEME_DEAR_LIGHT);
    REQUIRE(std::to_underlying(Theme::Ruby) == proto::THEME_RUBY);
    REQUIRE(std::to_underlying(Theme::Sapphire) == proto::THEME_SAPPHIRE);
    REQUIRE(std::to_underlying(Theme::Emerald) == proto::THEME_EMERALD);
    REQUIRE(std::to_underlying(Theme::Amethyst) == proto::THEME_AMETHYST);
    REQUIRE(std::to_underlying(Theme::Amber) == proto::THEME_AMBER);
    REQUIRE(std::to_underlying(Theme::Nocturnal) == proto::THEME_NOCTURNAL);
    REQUIRE(std::to_underlying(Theme::Ash) == proto::THEME_ASH);
    REQUIRE(std::to_underlying(Theme::Joker) == proto::THEME_JOKER);
    REQUIRE(std::to_underlying(Theme::Raspberry) == proto::THEME_RASPBERRY);
    REQUIRE(std::to_underlying(Theme::Stealth) == proto::THEME_STEALTH);
    REQUIRE(std::to_underlying(Theme::Gasoline) == proto::THEME_GASOLINE);
    REQUIRE(std::to_underlying(Theme::Bumblebee) == proto::THEME_BUMBLEBEE);
    REQUIRE(std::to_underlying(Theme::Lavender) == proto::THEME_LAVENDER);
    REQUIRE(std::to_underlying(Theme::Frost) == proto::THEME_FROST);
    REQUIRE(std::to_underlying(Theme::Rose) == proto::THEME_ROSE);
  }
}
