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

#include "core/formatters/entity_formatter.hpp"
#include "core/formatters/lang_formatter.hpp"
#include "core/formatters/menu_action_formatter.hpp"
#include "core/formatters/save_format_formatter.hpp"
#include "core/formatters/theme_formatter.hpp"
#include "core/formatters/tile_compression_formatter.hpp"
#include "core/formatters/tile_encoding_formatter.hpp"

using namespace tactile;

TEST_SUITE("CustomFormatters")
{
  TEST_CASE("Entities")
  {
    CHECK(fmt::format("{}", kNullEntity) == "Entity(NULL)");
    CHECK(fmt::format("{}", Entity {}) == "Entity(0)");
    CHECK(fmt::format("{}", Entity {42}) == "Entity(42)");
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
  }

  TEST_CASE("Menu actions")
  {
    CHECK(fmt::format("{}", MenuAction::ResizeMap) == "ResizeMap");
    CHECK(fmt::format("{}", MenuAction::OpenSettings) == "OpenSettings");
    CHECK(fmt::format("{}", MenuAction::FixInvalidTiles) == "FixInvalidTiles");
  }

  TEST_CASE("Tile compression strategies")
  {
    CHECK(fmt::format("{}", TileCompression::None) == "None");
    CHECK(fmt::format("{}", TileCompression::Zlib) == "Zlib");
    CHECK(fmt::format("{}", TileCompression::Zstd) == "Zstd");
  }

  TEST_CASE("Tile encodings")
  {
    CHECK(fmt::format("{}", TileEncoding::Plain) == "Plain");
    CHECK(fmt::format("{}", TileEncoding::Base64) == "Base64");
  }

  TEST_CASE("Save formats")
  {
    CHECK(fmt::format("{}", SaveFormat::TactileYaml) == "TactileYaml");
    CHECK(fmt::format("{}", SaveFormat::TiledJson) == "TiledJson");
    CHECK(fmt::format("{}", SaveFormat::TiledXml) == "TiledXml");
  }

  TEST_CASE_TEMPLATE("Magic enum formatters",
                     E,
                     Theme,
                     MenuAction,
                     TileCompression,
                     TileEncoding,
                     SaveFormat)
  {
    magic_enum::enum_for_each<E>([](const E value) {
      const auto str = fmt::format("{}", value);
      CHECK(!str.empty());
      CHECK(str != "?");
    });
  }
}
