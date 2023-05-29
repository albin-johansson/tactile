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
#include <fmt/format.h>
#include <magic_enum.hpp>

#include "common/fmt/entity_formatter.hpp"
#include "common/fmt/lang_formatter.hpp"
#include "common/fmt/theme_formatter.hpp"

namespace tactile::test {

TEST_SUITE("Custom formatters")
{
  TEST_CASE("Entities")
  {
    CHECK(fmt::format("{}", Entity {}) == "0");
    CHECK(fmt::format("{}", Entity {42}) == "42");
  }

  TEST_CASE("Languages")
  {
    CHECK(fmt::format("{}", Lang::EN) == "American English");
    CHECK(fmt::format("{}", Lang::EN_GB) == "British English");
    CHECK(fmt::format("{}", Lang::SV) == "Svenska");
  }

  TEST_CASE("Themes")
  {
    CHECK(fmt::format("{}", Theme::DearDark) == "DearDark");
    CHECK(fmt::format("{}", Theme::DearLight) == "DearLight");

    CHECK(fmt::format("{}", Theme::Ruby) == "Ruby");
    CHECK(fmt::format("{}", Theme::Sapphire) == "Sapphire");
    CHECK(fmt::format("{}", Theme::Emerald) == "Emerald");
    CHECK(fmt::format("{}", Theme::Amethyst) == "Amethyst");
    CHECK(fmt::format("{}", Theme::Amber) == "Amber");
    CHECK(fmt::format("{}", Theme::Nocturnal) == "Nocturnal");
    CHECK(fmt::format("{}", Theme::Ash) == "Ash");
    CHECK(fmt::format("{}", Theme::Diamond) == "Diamond");
    CHECK(fmt::format("{}", Theme::Joker) == "Joker");
    CHECK(fmt::format("{}", Theme::Raspberry) == "Raspberry");
    CHECK(fmt::format("{}", Theme::Stealth) == "Stealth");
    CHECK(fmt::format("{}", Theme::Vanilla) == "Vanilla");
    CHECK(fmt::format("{}", Theme::Gasoline) == "Gasoline");
    CHECK(fmt::format("{}", Theme::Bumblebee) == "Bumblebee");
    CHECK(fmt::format("{}", Theme::Lavender) == "Lavender");
    CHECK(fmt::format("{}", Theme::Frost) == "Frost");
    CHECK(fmt::format("{}", Theme::Rose) == "Rose");

    magic_enum::enum_for_each<Theme>([](const Theme theme) {
      const auto str = fmt::format("{}", theme);
      CHECK(!str.empty());
      CHECK(str != "?");
    });
  }
}

}  // namespace tactile::test
