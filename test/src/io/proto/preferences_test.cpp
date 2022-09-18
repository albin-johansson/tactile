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

#include <gtest/gtest.h>

#include "settings.pb.h"

using namespace tactile::io;
using namespace tactile::ui;

namespace tactile::test {

TEST(Preferences, Defaults)
{
  const PreferenceState state;

  ASSERT_EQ(def_preferred_format, state.preferred_format);
  ASSERT_EQ(def_theme, state.theme);
  ASSERT_EQ(def_viewport_bg, state.viewport_background);
  ASSERT_EQ(def_command_capacity, state.command_capacity);
  ASSERT_EQ(def_preferred_tile_size, state.preferred_tile_size);
  ASSERT_EQ(def_font_size, state.font_size);
  ASSERT_EQ(def_viewport_overlay_pos, state.viewport_overlay_pos);
  ASSERT_EQ(def_embed_tilesets, state.embed_tilesets);
  ASSERT_EQ(def_indent_output, state.indent_output);
  ASSERT_EQ(def_fold_tile_data, state.fold_tile_data);
  ASSERT_EQ(def_show_grid, state.show_grid);
  ASSERT_EQ(def_show_layer_dock, state.show_layer_dock);
  ASSERT_EQ(def_show_tileset_dock, state.show_tileset_dock);
  ASSERT_EQ(def_show_property_dock, state.show_property_dock);
  ASSERT_EQ(def_show_component_dock, state.show_component_dock);
  ASSERT_EQ(def_show_log_dock, state.show_log_dock);
  ASSERT_EQ(def_window_border, state.window_border);
  ASSERT_EQ(def_restore_layout, state.restore_layout);
  ASSERT_EQ(def_restore_last_session, state.restore_last_session);
  ASSERT_EQ(def_show_viewport_overlay_fps, state.show_viewport_overlay_fps);
  ASSERT_EQ(def_highlight_active_layer, state.highlight_active_layer);
  ASSERT_EQ(def_use_default_font, state.use_default_font);
}

TEST(Preferences, EnsureThemeEnumsMatch)
{
  ASSERT_EQ(proto::THEME_DEAR_DARK, std::to_underlying(EditorTheme::DearDark));
  ASSERT_EQ(proto::THEME_DEAR_LIGHT, std::to_underlying(EditorTheme::DearLight));
  ASSERT_EQ(proto::THEME_RUBY, std::to_underlying(EditorTheme::Ruby));
  ASSERT_EQ(proto::THEME_SAPPHIRE, std::to_underlying(EditorTheme::Sapphire));
  ASSERT_EQ(proto::THEME_EMERALD, std::to_underlying(EditorTheme::Emerald));
  ASSERT_EQ(proto::THEME_AMETHYST, std::to_underlying(EditorTheme::Amethyst));
  ASSERT_EQ(proto::THEME_AMBER, std::to_underlying(EditorTheme::Amber));
  ASSERT_EQ(proto::THEME_NOCTURNAL, std::to_underlying(EditorTheme::Nocturnal));
  ASSERT_EQ(proto::THEME_ASH, std::to_underlying(EditorTheme::Ash));
  ASSERT_EQ(proto::THEME_DIAMOND, std::to_underlying(EditorTheme::Diamond));
  ASSERT_EQ(proto::THEME_JOKER, std::to_underlying(EditorTheme::Joker));
  ASSERT_EQ(proto::THEME_RASPBERRY, std::to_underlying(EditorTheme::Raspberry));
}

TEST(Preferences, EnsureOverlayPosEnumsMatch)
{
  ASSERT_EQ(proto::OVERLAY_POS_TOP_LEFT, std::to_underlying(OverlayPos::TopLeft));
  ASSERT_EQ(proto::OVERLAY_POS_TOP_RIGHT, std::to_underlying(OverlayPos::TopRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_RIGHT, std::to_underlying(OverlayPos::BottomRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_LEFT, std::to_underlying(OverlayPos::BottomLeft));
}

}  // namespace tactile::test
