#include "io/persistence/preference_state.hpp"

#include <gtest/gtest.h>

#include "settings.pb.h"

using namespace tactile;

TEST(PreferenceState, Defaults)
{
  const preference_state state;

  ASSERT_EQ(Theme::nocturnal, state.get_theme());

  ASSERT_TRUE(state.is_grid_visible());
  ASSERT_FALSE(state.has_window_border());

  ASSERT_TRUE(state.is_tileset_dock_visible());
  ASSERT_TRUE(state.is_properties_dock_visible());
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
  preference_state state;

  state.set_theme(Theme::amber);
  ASSERT_EQ(Theme::amber, state.get_theme());
}

TEST(PreferenceState, SetViewportBg)
{
  preference_state state;

  state.set_viewport_bg(cen::colors::rebecca_purple);
  ASSERT_EQ(cen::colors::rebecca_purple, state.viewport_bg());
}

TEST(PreferenceState, SetWindowBorder)
{
  preference_state state;

  state.set_window_border(true);
  ASSERT_TRUE(state.has_window_border());

  state.set_window_border(false);
  ASSERT_FALSE(state.has_window_border());
}

TEST(PreferenceState, SetLayerDockVisible)
{
  preference_state state;

  state.set_layer_dock_visible(true);
  ASSERT_TRUE(state.is_layer_dock_visible());

  state.set_layer_dock_visible(false);
  ASSERT_FALSE(state.is_layer_dock_visible());
}

TEST(PreferenceState, SetTilesetDockVisible)
{
  preference_state state;

  state.set_tileset_dock_visible(true);
  ASSERT_TRUE(state.is_tileset_dock_visible());

  state.set_tileset_dock_visible(false);
  ASSERT_FALSE(state.is_tileset_dock_visible());
}

TEST(PreferenceState, SetPropertiesDockVisible)
{
  preference_state state;

  state.set_properties_dock_visible(true);
  ASSERT_TRUE(state.is_properties_dock_visible());

  state.set_properties_dock_visible(false);
  ASSERT_FALSE(state.is_properties_dock_visible());
}

TEST(PreferenceState, SetComponentDockVisible)
{
  preference_state state;

  state.set_component_dock_visible(true);
  ASSERT_TRUE(state.is_component_dock_visible());

  state.set_component_dock_visible(false);
  ASSERT_FALSE(state.is_component_dock_visible());
}

TEST(PreferenceState, SetLogDockVisible)
{
  preference_state state;

  state.set_log_dock_visible(true);
  ASSERT_TRUE(state.is_log_dock_visible());

  state.set_log_dock_visible(false);
  ASSERT_FALSE(state.is_log_dock_visible());
}

TEST(PreferenceState, SetGridVisible)
{
  preference_state state;

  state.set_grid_visible(true);
  ASSERT_TRUE(state.is_grid_visible());

  state.set_grid_visible(false);
  ASSERT_FALSE(state.is_grid_visible());
}

TEST(PreferenceState, SetEmbedTilesets)
{
  preference_state state;

  state.set_embed_tilesets(true);
  ASSERT_TRUE(state.embed_tilesets());

  state.set_embed_tilesets(false);
  ASSERT_FALSE(state.embed_tilesets());
}

TEST(PreferenceState, SetFoldTileData)
{
  preference_state state;

  state.set_fold_tile_data(true);
  ASSERT_TRUE(state.fold_tile_data());

  state.set_fold_tile_data(false);
  ASSERT_FALSE(state.fold_tile_data());
}

TEST(PreferenceState, SetIndentOutput)
{
  preference_state state;

  state.set_indent_output(true);
  ASSERT_TRUE(state.indent_output());

  state.set_indent_output(false);
  ASSERT_FALSE(state.indent_output());
}

TEST(PreferenceState, SetWillRestoreLayout)
{
  preference_state state;

  state.set_will_restore_layout(true);
  ASSERT_TRUE(state.will_restore_layout());

  state.set_will_restore_layout(false);
  ASSERT_FALSE(state.will_restore_layout());
}

TEST(PreferenceState, SetWillRestoreLastSession)
{
  preference_state state;

  state.set_will_restore_last_session(true);
  ASSERT_TRUE(state.will_restore_last_session());

  state.set_will_restore_last_session(false);
  ASSERT_FALSE(state.will_restore_last_session());
}

TEST(PreferenceState, SetViewportOverlayPos)
{
  preference_state state;

  state.set_viewport_overlay_pos(overlay_pos::bottom_right);
  ASSERT_EQ(overlay_pos::bottom_right, state.viewport_overlay_pos());
}

TEST(PreferenceState, SetCommandCapacity)
{
  preference_state state;

  state.set_command_capacity(42u);
  ASSERT_EQ(42u, state.command_capacity());
}

TEST(PreferenceState, SetPreferredFormat)
{
  preference_state state;

  state.set_preferred_format("JSON");
  ASSERT_EQ("JSON", state.preferred_format());
}

TEST(PreferenceState, SetPreferredTileWidth)
{
  preference_state state;

  state.set_preferred_tile_width(128);
  ASSERT_EQ(128, state.preferred_tile_width());
  ASSERT_EQ(32, state.preferred_tile_height());
}

TEST(PreferenceState, SetPreferredTileHeight)
{
  preference_state state;

  state.set_preferred_tile_height(853);
  ASSERT_EQ(32, state.preferred_tile_width());
  ASSERT_EQ(853, state.preferred_tile_height());
}

TEST(PreferenceState, EnsureThemeEnumsMatch)
{
  ASSERT_EQ(proto::DEAR_DARK, cen::to_underlying(Theme::dear_dark));
  ASSERT_EQ(proto::DEAR_LIGHT, cen::to_underlying(Theme::dear_light));
  ASSERT_EQ(proto::RUBY, cen::to_underlying(Theme::ruby));
  ASSERT_EQ(proto::SAPPHIRE, cen::to_underlying(Theme::sapphire));
  ASSERT_EQ(proto::EMERALD, cen::to_underlying(Theme::emerald));
  ASSERT_EQ(proto::AMETHYST, cen::to_underlying(Theme::amethyst));
  ASSERT_EQ(proto::AMBER, cen::to_underlying(Theme::amber));
  ASSERT_EQ(proto::NOCTURNAL, cen::to_underlying(Theme::nocturnal));
  ASSERT_EQ(proto::ASH, cen::to_underlying(Theme::ash));
}

TEST(PreferenceState, EnsureOverlayPosEnumsMatch)
{
  ASSERT_EQ(proto::TOP_LEFT, cen::to_underlying(overlay_pos::top_left));
  ASSERT_EQ(proto::TOP_RIGHT, cen::to_underlying(overlay_pos::top_right));
  ASSERT_EQ(proto::BOTTOM_RIGHT, cen::to_underlying(overlay_pos::bottom_right));
  ASSERT_EQ(proto::BOTTOM_LEFT, cen::to_underlying(overlay_pos::bottom_left));
}