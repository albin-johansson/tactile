#include "io/persistence/preference_state.hpp"

#include <centurion/color.hpp>
#include <gtest/gtest.h>

#include "core/common/enum.hpp"
#include "settings.pb.h"

using namespace tactile;

TEST(PreferenceState, Defaults)
{
  const io::PreferenceState state;

  ASSERT_EQ(EditorTheme::Nocturnal, state.get_theme());

  ASSERT_TRUE(state.is_grid_visible());
  ASSERT_FALSE(state.has_window_border());

  ASSERT_TRUE(state.is_tileset_dock_visible());
  ASSERT_TRUE(state.is_property_dock_visible());
  ASSERT_TRUE(state.is_component_dock_visible());
  ASSERT_TRUE(state.is_layer_dock_visible());
  ASSERT_FALSE(state.is_log_dock_visible());

  ASSERT_TRUE(state.indent_output());
  ASSERT_FALSE(state.fold_tile_data());
  ASSERT_FALSE(state.embed_tilesets());

  ASSERT_EQ("YAML", state.preferred_format());
  ASSERT_EQ(32, state.preferred_tile_width());
  ASSERT_EQ(32, state.preferred_tile_height());

  ASSERT_TRUE(state.will_restore_layout());
  ASSERT_TRUE(state.will_restore_last_session());
}

TEST(PreferenceState, SetTheme)
{
  io::PreferenceState state;

  state.set_theme(EditorTheme::Amber);
  ASSERT_EQ(EditorTheme::Amber, state.get_theme());
}

TEST(PreferenceState, SetViewportBg)
{
  io::PreferenceState state;

  state.set_viewport_bg(cen::colors::rebecca_purple);
  ASSERT_EQ(cen::colors::rebecca_purple, state.viewport_bg());
}

TEST(PreferenceState, SetWindowBorder)
{
  io::PreferenceState state;

  state.set_window_border(true);
  ASSERT_TRUE(state.has_window_border());

  state.set_window_border(false);
  ASSERT_FALSE(state.has_window_border());
}

TEST(PreferenceState, SetLayerDockVisible)
{
  io::PreferenceState state;

  state.set_layer_dock_visible(true);
  ASSERT_TRUE(state.is_layer_dock_visible());

  state.set_layer_dock_visible(false);
  ASSERT_FALSE(state.is_layer_dock_visible());
}

TEST(PreferenceState, SetTilesetDockVisible)
{
  io::PreferenceState state;

  state.set_tileset_dock_visible(true);
  ASSERT_TRUE(state.is_tileset_dock_visible());

  state.set_tileset_dock_visible(false);
  ASSERT_FALSE(state.is_tileset_dock_visible());
}

TEST(PreferenceState, SetPropertiesDockVisible)
{
  io::PreferenceState state;

  state.set_property_dock_visible(true);
  ASSERT_TRUE(state.is_property_dock_visible());

  state.set_property_dock_visible(false);
  ASSERT_FALSE(state.is_property_dock_visible());
}

TEST(PreferenceState, SetComponentDockVisible)
{
  io::PreferenceState state;

  state.set_component_dock_visible(true);
  ASSERT_TRUE(state.is_component_dock_visible());

  state.set_component_dock_visible(false);
  ASSERT_FALSE(state.is_component_dock_visible());
}

TEST(PreferenceState, SetLogDockVisible)
{
  io::PreferenceState state;

  state.set_log_dock_visible(true);
  ASSERT_TRUE(state.is_log_dock_visible());

  state.set_log_dock_visible(false);
  ASSERT_FALSE(state.is_log_dock_visible());
}

TEST(PreferenceState, SetGridVisible)
{
  io::PreferenceState state;

  state.set_grid_visible(true);
  ASSERT_TRUE(state.is_grid_visible());

  state.set_grid_visible(false);
  ASSERT_FALSE(state.is_grid_visible());
}

TEST(PreferenceState, SetEmbedTilesets)
{
  io::PreferenceState state;

  state.set_embed_tilesets(true);
  ASSERT_TRUE(state.embed_tilesets());

  state.set_embed_tilesets(false);
  ASSERT_FALSE(state.embed_tilesets());
}

TEST(PreferenceState, SetFoldTileData)
{
  io::PreferenceState state;

  state.set_fold_tile_data(true);
  ASSERT_TRUE(state.fold_tile_data());

  state.set_fold_tile_data(false);
  ASSERT_FALSE(state.fold_tile_data());
}

TEST(PreferenceState, SetIndentOutput)
{
  io::PreferenceState state;

  state.set_indent_output(true);
  ASSERT_TRUE(state.indent_output());

  state.set_indent_output(false);
  ASSERT_FALSE(state.indent_output());
}

TEST(PreferenceState, SetWillRestoreLayout)
{
  io::PreferenceState state;

  state.set_will_restore_layout(true);
  ASSERT_TRUE(state.will_restore_layout());

  state.set_will_restore_layout(false);
  ASSERT_FALSE(state.will_restore_layout());
}

TEST(PreferenceState, SetWillRestoreLastSession)
{
  io::PreferenceState state;

  state.set_will_restore_last_session(true);
  ASSERT_TRUE(state.will_restore_last_session());

  state.set_will_restore_last_session(false);
  ASSERT_FALSE(state.will_restore_last_session());
}

TEST(PreferenceState, SetViewportOverlayPos)
{
  io::PreferenceState state;

  state.set_viewport_overlay_pos(io::OverlayPos::BottomRight);
  ASSERT_EQ(io::OverlayPos::BottomRight, state.viewport_overlay_pos());
}

TEST(PreferenceState, SetCommandCapacity)
{
  io::PreferenceState state;

  state.set_command_capacity(42u);
  ASSERT_EQ(42u, state.command_capacity());
}

TEST(PreferenceState, SetPreferredFormat)
{
  io::PreferenceState state;

  state.set_preferred_format("JSON");
  ASSERT_EQ("JSON", state.preferred_format());
}

TEST(PreferenceState, SetPreferredTileWidth)
{
  io::PreferenceState state;

  state.set_preferred_tile_width(128);
  ASSERT_EQ(128, state.preferred_tile_width());
  ASSERT_EQ(32, state.preferred_tile_height());
}

TEST(PreferenceState, SetPreferredTileHeight)
{
  io::PreferenceState state;

  state.set_preferred_tile_height(853);
  ASSERT_EQ(32, state.preferred_tile_width());
  ASSERT_EQ(853, state.preferred_tile_height());
}

TEST(PreferenceState, EnsureThemeEnumsMatch)
{
  ASSERT_EQ(proto::THEME_DEAR_DARK, to_underlying(EditorTheme::DearDark));
  ASSERT_EQ(proto::THEME_DEAR_LIGHT, to_underlying(EditorTheme::DearLight));
  ASSERT_EQ(proto::THEME_RUBY, to_underlying(EditorTheme::Ruby));
  ASSERT_EQ(proto::THEME_SAPPHIRE, to_underlying(EditorTheme::Sapphire));
  ASSERT_EQ(proto::THEME_EMERALD, to_underlying(EditorTheme::Emerald));
  ASSERT_EQ(proto::THEME_AMETHYST, to_underlying(EditorTheme::Amethyst));
  ASSERT_EQ(proto::THEME_AMBER, to_underlying(EditorTheme::Amber));
  ASSERT_EQ(proto::THEME_NOCTURNAL, to_underlying(EditorTheme::Nocturnal));
  ASSERT_EQ(proto::THEME_ASH, to_underlying(EditorTheme::Ash));
  ASSERT_EQ(proto::THEME_DIAMOND, to_underlying(EditorTheme::Diamond));
  ASSERT_EQ(proto::THEME_JOKER, to_underlying(EditorTheme::Joker));
  ASSERT_EQ(proto::THEME_RASPBERRY, to_underlying(EditorTheme::Raspberry));
}

TEST(PreferenceState, EnsureOverlayPosEnumsMatch)
{
  ASSERT_EQ(proto::OVERLAY_POS_TOP_LEFT, to_underlying(io::OverlayPos::TopLeft));
  ASSERT_EQ(proto::OVERLAY_POS_TOP_RIGHT, to_underlying(io::OverlayPos::TopRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_RIGHT, to_underlying(io::OverlayPos::BottomRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_LEFT, to_underlying(io::OverlayPos::BottomLeft));
}