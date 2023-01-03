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

#include "io/proto/preferences.hpp"

#include <utility>  // to_underlying

#include <doctest/doctest.h>

#include "settings.pb.h"

using namespace tactile::io;
using namespace tactile::ui;

namespace tactile::test {

TEST_SUITE("Settings")
{
  TEST_CASE("Defaults")
  {
    const PreferenceState state;

    REQUIRE(def_preferred_format == state.preferred_format);
    REQUIRE(def_theme == state.theme);
    REQUIRE(def_viewport_bg == state.viewport_background);
    REQUIRE(def_grid_color == state.grid_color);
    REQUIRE(def_command_capacity == state.command_capacity);
    REQUIRE(def_preferred_tile_size == state.preferred_tile_size);
    REQUIRE(def_font_size == state.font_size);
    REQUIRE(def_viewport_overlay_pos == state.viewport_overlay_pos);
    REQUIRE(def_embed_tilesets == state.embed_tilesets);
    REQUIRE(def_indent_output == state.indent_output);
    REQUIRE(def_fold_tile_data == state.fold_tile_data);
    REQUIRE(def_show_grid == state.show_grid);
    REQUIRE(def_show_layer_dock == state.show_layer_dock);
    REQUIRE(def_show_tileset_dock == state.show_tileset_dock);
    REQUIRE(def_show_property_dock == state.show_property_dock);
    REQUIRE(def_show_component_dock == state.show_component_dock);
    REQUIRE(def_show_log_dock == state.show_log_dock);
    REQUIRE(def_window_border == state.window_border);
    REQUIRE(def_restore_layout == state.restore_layout);
    REQUIRE(def_restore_last_session == state.restore_last_session);
    REQUIRE(def_show_viewport_overlay_fps == state.show_viewport_overlay_fps);
    REQUIRE(def_highlight_active_layer == state.highlight_active_layer);
    REQUIRE(def_use_default_font == state.use_default_font);
  }

  TEST_CASE("Ensure theme enums match")
  {
    REQUIRE(proto::THEME_DEAR_DARK == std::to_underlying(EditorTheme::DearDark));
    REQUIRE(proto::THEME_DEAR_LIGHT == std::to_underlying(EditorTheme::DearLight));
    REQUIRE(proto::THEME_RUBY == std::to_underlying(EditorTheme::Ruby));
    REQUIRE(proto::THEME_SAPPHIRE == std::to_underlying(EditorTheme::Sapphire));
    REQUIRE(proto::THEME_EMERALD == std::to_underlying(EditorTheme::Emerald));
    REQUIRE(proto::THEME_AMETHYST == std::to_underlying(EditorTheme::Amethyst));
    REQUIRE(proto::THEME_AMBER == std::to_underlying(EditorTheme::Amber));
    REQUIRE(proto::THEME_NOCTURNAL == std::to_underlying(EditorTheme::Nocturnal));
    REQUIRE(proto::THEME_ASH == std::to_underlying(EditorTheme::Ash));
    REQUIRE(proto::THEME_JOKER == std::to_underlying(EditorTheme::Joker));
    REQUIRE(proto::THEME_RASPBERRY == std::to_underlying(EditorTheme::Raspberry));
    REQUIRE(proto::THEME_STEALTH == std::to_underlying(EditorTheme::Stealth));
    REQUIRE(proto::THEME_GASOLINE == std::to_underlying(EditorTheme::Gasoline));
    REQUIRE(proto::THEME_BUMBLEBEE == std::to_underlying(EditorTheme::Bumblebee));
    REQUIRE(proto::THEME_LAVENDER == std::to_underlying(EditorTheme::Lavender));
    REQUIRE(proto::THEME_FROST == std::to_underlying(EditorTheme::Frost));
    REQUIRE(proto::THEME_ROSE == std::to_underlying(EditorTheme::Rose));
  }

  TEST_CASE("Ensure overlay position enums match")
  {
    REQUIRE(proto::OVERLAY_POS_TOP_LEFT == std::to_underlying(OverlayPos::TopLeft));
    REQUIRE(proto::OVERLAY_POS_TOP_RIGHT == std::to_underlying(OverlayPos::TopRight));
    REQUIRE(proto::OVERLAY_POS_BOTTOM_RIGHT ==
            std::to_underlying(OverlayPos::BottomRight));
    REQUIRE(proto::OVERLAY_POS_BOTTOM_LEFT == std::to_underlying(OverlayPos::BottomLeft));
  }
}

}  // namespace tactile::test
